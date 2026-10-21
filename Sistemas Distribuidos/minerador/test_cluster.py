"""Testes do coordenador e dos agentes com fonte simulada, sem acessar a LAN."""
import contextlib
import io
import json
from pathlib import Path
import tempfile
import threading
import time
import unittest
from unittest.mock import patch

from agente_cluster import Agente
from cluster.nucleo import Coordenador, ErroAPI, chamar_http
from cluster.servidor import Servidor
from minerador import MineracaoParalela
from protocolo import hash_bloco, prova_valida

TRABALHO = {"id": "bloco-a", "altura": 1, "dados": "ação ☕", "dificuldade": 0}


def temporario():
    pasta = Path(__file__).resolve().parent / ".cluster" / "testes"
    pasta.mkdir(parents=True, exist_ok=True)
    return tempfile.TemporaryDirectory(dir=pasta)


def mensagem(i=1, **extras):
    return {"id": f"pc-{i:02d}", "sessao": f"sessao-de-teste-{i:02d}", "sequencia": 1,
            "tentativas": 0, "relato": {}, **extras}


class TestCoordenacao(unittest.TestCase):
    def setUp(self):
        self.tmp = temporario()
        self.envios = []

        def http(base, rota, dados=None):
            self.envios.append((rota, dados))
            return 201, {"ok": True}

        self.coord = Coordenador(Path(self.tmp.name) / "config.json", http=http)
        self.coord.controlar("iniciar")
        with self.coord.lock:
            self.coord.atualizar_fonte(TRABALHO)

    def tearDown(self):
        self.coord.fechar()
        self.tmp.cleanup()

    def test_seis_pcs_recebem_mesma_rodada_com_slots_diferentes(self):
        respostas = [self.coord.sincronizar(mensagem(i)) for i in range(1, 7)]
        self.assertEqual({r["config"]["slot"] for r in respostas}, set(range(6)))
        self.assertEqual(len({r["trabalho"]["geracao"] for r in respostas}), 1)
        self.assertEqual([r["config"]["motor"] for r in respostas], ["auto"] * 2 + ["cuda"] * 4)
        # Diferentes quantidades de processos e volta do contador mantêm os seis conjuntos separados.
        for base in (123, 2**64 - 5):
            grupos = []
            for slot, quantidade in enumerate((2, 7, 1, 1, 1, 1)):
                passo = quantidade * 6
                limite = (2**64 // passo) * passo
                nonces = {(base + slot + j * 6 + k * passo) % limite
                          for j in range(quantidade) for k in range(20)}
                self.assertTrue(all((n - base) % 6 == slot for n in nonces))
                grupos.append(nonces)
            for i, grupo in enumerate(grupos):
                self.assertTrue(all(not grupo & outro for outro in grupos[i+1:]))

    def test_pausa_nova_rodada_e_desconexao_invalidam_trabalho(self):
        anterior = self.coord.sincronizar(mensagem())["trabalho"]
        self.coord.configurar_no("pc-01", {"habilitado": False})
        self.assertIsNone(self.coord.sincronizar(mensagem())["trabalho"])
        self.coord.configurar_no("pc-01", {"habilitado": True})
        self.coord.atualizar_fonte(dict(TRABALHO, id="novo"))
        atual = self.coord.sincronizar(mensagem())["trabalho"]
        self.assertNotEqual(anterior["geracao"], atual["geracao"])
        with self.assertRaisesRegex(ErroAPI, "encerrada"):
            self.coord.solucao(mensagem(geracao=anterior["geracao"], nonce=anterior["inicio_nonce"]))
        self.coord.falha_fonte(OSError("Conexão recusada"))
        self.assertIsNone(self.coord.sincronizar(mensagem())["trabalho"])
        self.coord.atualizar_fonte(TRABALHO)
        self.coord.controlar("pausar")
        self.assertIsNone(self.coord.sincronizar(mensagem())["trabalho"])

    def test_id_duplicado_rejeitado_e_reconexao_expirada_permitida(self):
        self.coord.sincronizar(mensagem())
        with self.assertRaisesRegex(ErroAPI, "já está conectado"):
            self.coord.sincronizar(mensagem(sessao="outra-sessao-de-teste"))
        self.coord.nos["pc-01"]["visto"] -= 11
        self.coord.sincronizar(mensagem(sessao="outra-sessao-de-teste"))
        self.assertEqual(self.coord.estado()["nos"][0]["online"], True)

    def test_contadores_idempotentes_e_velocidade_apenas_da_rodada_atual(self):
        job = self.coord.job
        dados = mensagem(tentativas=123, relato={"velocidade": 456, "estado": "minerando", "job": job["geracao"]})
        self.coord.sincronizar(dados)
        self.coord.sincronizar(dados)
        self.assertEqual(self.coord.estado()["tentativas"], "123")
        self.assertEqual(self.coord.estado()["velocidade"], 456)
        self.coord.controlar("pausar")
        self.assertEqual(self.coord.estado()["velocidade"], 0)

    def test_prova_validada_enviada_uma_vez_e_trabalho_aceito_nao_reinicia(self):
        job = self.coord.sincronizar(mensagem())["trabalho"]
        dados = mensagem(geracao=job["geracao"], nonce=job["inicio_nonce"])
        self.assertTrue(self.coord.solucao(dados)["aceito"])
        self.assertEqual(self.envios, [("/solucao", {"id": TRABALHO["id"], "minerador": "Cluster", "nonce": int(job["inicio_nonce"])})])
        with self.assertRaises(ErroAPI):
            self.coord.solucao(dados)
        self.coord.atualizar_fonte(TRABALHO)
        self.assertIsNone(self.coord.job)
        self.assertEqual(self.coord.aceitos, 1)

    def test_rejeita_particao_errada_hash_invalido_e_contadores_invalidos(self):
        job = self.coord.sincronizar(mensagem())["trabalho"]
        with self.assertRaisesRegex(ErroAPI, "partição"):
            self.coord.solucao(mensagem(geracao=job["geracao"], nonce=str(int(job["inicio_nonce"]) + 1)))
        self.coord.atualizar_fonte(dict(TRABALHO, dificuldade=64))
        job = self.coord.job
        with self.assertRaisesRegex(ErroAPI, "inválida"):
            self.coord.solucao(mensagem(geracao=job["geracao"], nonce=job["inicio_nonce"]))
        for valor in (-1, float("nan"), float("inf")):
            with self.assertRaises(ErroAPI):
                self.coord.sincronizar(mensagem(sequencia=2, relato={"velocidade": valor}))
        self.assertEqual(self.envios, [])

    def test_envios_simultaneos_nao_duplicam_prova(self):
        entrou, liberar = threading.Event(), threading.Event()
        http = self.coord.http

        def bloqueado(*args):
            entrou.set()
            liberar.wait(3)
            return http(*args)

        self.coord.http = bloqueado
        job = self.coord.sincronizar(mensagem())["trabalho"]
        dados = mensagem(geracao=job["geracao"], nonce=job["inicio_nonce"])
        thread = threading.Thread(target=lambda: self.coord.solucao(dados))
        thread.start()
        try:
            self.assertTrue(entrou.wait(2))
            with self.assertRaisesRegex(ErroAPI, "envio"):
                self.coord.solucao(dados)
        finally:
            liberar.set()
            thread.join(4)
        self.assertEqual(len(self.envios), 1)

    def test_config_persiste_mas_segredos_nao_aparecem_no_estado(self):
        self.coord.configurar_no("pc-01", {"nome": "Laboratório", "processos": 3})
        novo = Coordenador(self.coord.arquivo)
        self.assertEqual(novo.config["nos"][0]["processos"], 3)
        self.assertEqual(novo.config["token_admin"], self.coord.config["token_admin"])
        self.assertNotIn(self.coord.config["token_admin"], json.dumps(novo.estado()))
        self.assertNotIn(self.coord.config["token_agente"], json.dumps(novo.estado()))
        self.assertFalse(novo.ativo)


class TestPoolCluster(unittest.TestCase):
    def test_pool_real_com_diferentes_quantidades_e_wrap(self):
        vistos = set()
        for slot, quantidade in ((0, 2), (1, 3)):
            pool = MineracaoParalela(quantidade, motor="auto")
            try:
                pool.iniciar(TRABALHO, "Cluster", inicio_nonce=2**64-5, particao=slot, particoes=6)
                prazo = time.monotonic() + 10
                solucoes = []
                while len(solucoes) < quantidade and time.monotonic() < prazo:
                    solucoes.extend(pool.consultar())
                    time.sleep(0.01)
                self.assertEqual(len(solucoes), quantidade)
                for nonce, digest in solucoes:
                    self.assertEqual((nonce-(2**64-5)) % 6, slot)
                    self.assertNotIn(nonce, vistos)
                    vistos.add(nonce)
                    self.assertEqual(digest, hash_bloco(TRABALHO, "Cluster", nonce))
            finally:
                pool.fechar()


class TestHTTPCluster(unittest.TestCase):
    def setUp(self):
        self.tmp = temporario()
        self.coord = Coordenador(Path(self.tmp.name) / "config.json")
        self.server = Servidor(("127.0.0.1", 0), self.coord)
        self.thread = threading.Thread(target=self.server.serve_forever, daemon=True)
        self.thread.start()
        self.url = f"http://127.0.0.1:{self.server.server_port}"

    def tearDown(self):
        self.coord.fechar()
        self.server.shutdown()
        self.server.server_close()
        self.thread.join(2)
        self.tmp.cleanup()

    def test_autenticacao_segregacao_de_chaves_e_download_config(self):
        codigo, _ = chamar_http(self.url, "/api/estado")
        self.assertEqual(codigo, 401)
        self.assertEqual(chamar_http(self.url, "/api/estado", token=self.coord.config["token_agente"])[0], 401)
        self.assertEqual(chamar_http(self.url, "/api/controle", {"acao": "iniciar"}, token=self.coord.config["token_agente"])[0], 401)
        self.assertEqual(chamar_http(self.url, "/api/agente/sincronizar", mensagem(), token=self.coord.config["token_admin"])[0], 401)
        codigo, arquivo = chamar_http(self.url, "/api/configuracao/pc-01", token=self.coord.config["token_admin"])
        self.assertEqual(codigo, 200)
        self.assertEqual(arquivo["id"], "pc-01")
        self.assertEqual(arquivo["token"], self.coord.config["token_agente"])

    def test_seis_agentes_pela_api_e_controle_individual(self):
        self.coord.controlar("iniciar")
        self.coord.atualizar_fonte(TRABALHO)
        for i in range(1, 7):
            codigo, dados = chamar_http(self.url, "/api/agente/sincronizar", mensagem(i), token=self.coord.config["token_agente"])
            self.assertEqual(codigo, 200)
            self.assertEqual(dados["config"]["slot"], i-1)
        self.assertEqual(sum(n["online"] for n in self.coord.estado()["nos"]), 6)
        chamar_http(self.url, "/api/nos/pc-04", {"habilitado": False}, token=self.coord.config["token_admin"])
        self.assertIsNone(chamar_http(self.url, "/api/agente/sincronizar", mensagem(4), token=self.coord.config["token_agente"])[1]["trabalho"])

    def exercitar_agente(self, motor):
        from http.server import BaseHTTPRequestHandler, ThreadingHTTPServer
        aceitou = threading.Event()
        envios = []
        estado = {"trabalho": dict(TRABALHO, dificuldade=64)}

        class Fonte(BaseHTTPRequestHandler):
            def log_message(self, *args):
                pass

            def responder(self, codigo, dados):
                corpo = json.dumps(dados).encode()
                self.send_response(codigo)
                self.send_header("Content-Length", str(len(corpo)))
                self.end_headers()
                self.wfile.write(corpo)

            def do_GET(self):
                self.responder(200, estado)

            def do_POST(self):
                dados = json.loads(self.rfile.read(int(self.headers["Content-Length"])))
                if self.path == "/solucao":
                    envios.append(dados)
                    valida = dados["id"] == TRABALHO["id"] and prova_valida(TRABALHO, dados["minerador"], dados["nonce"])
                    self.responder(201 if valida else 400, {})
                    if valida:
                        aceitou.set()
                        estado["trabalho"] = None
                else:
                    self.responder(200, {})

        fonte = ThreadingHTTPServer(("127.0.0.1", 0), Fonte)
        thread = threading.Thread(target=fonte.serve_forever, daemon=True)
        thread.start()
        self.coord.configurar({"fonte": f"http://127.0.0.1:{fonte.server_port}", "nome": "Equipe ☕"})
        self.coord.configurar_no("pc-01", {"motor": motor, "processos": 1})
        self.coord.controlar("iniciar")
        self.coord.iniciar()
        agente = Agente({"id": "pc-01", "coordenador": self.url, "token": self.coord.config["token_agente"]})
        try:
            prazo = time.monotonic() + 25
            virou = False
            with contextlib.redirect_stdout(io.StringIO()):
                while not aceitou.is_set() and time.monotonic() < prazo:
                    agente.ciclo()
                    if agente.tentativas > 0 and not virou:
                        estado["trabalho"] = TRABALHO
                        virou = True
                    time.sleep(0.2)
            self.assertTrue(virou, "O agente deve trabalhar e reportar antes da troca de rodada.")
            self.assertTrue(aceitou.is_set(), "Nenhuma solução foi aceita pela fonte simulada.")
            self.assertEqual(self.coord.aceitos, 1)
            self.assertEqual(len(envios), 1)
            self.assertEqual(envios[0]["minerador"], "Equipe ☕")
            self.assertIsNone(self.coord.job)
            agente.ciclo()
            self.assertIsNone(agente.job)
            self.assertGreater(agente.tentativas, 0)
        finally:
            agente.parar(fechar=True)
            fonte.shutdown()
            fonte.server_close()
            thread.join(2)

    def test_agente_cpu_real_coordenador_e_fonte_http(self):
        self.exercitar_agente("auto")

    def test_agente_cuda_real_coordenador_e_fonte_http(self):
        from cuda_minerador import informacoes_cuda
        try:
            informacoes_cuda()
        except (OSError, RuntimeError) as erro:
            self.skipTest(str(erro))
        self.exercitar_agente("cuda")


if __name__ == "__main__":
    unittest.main()
