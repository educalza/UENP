"""Verificações locais, sem acessar o servidor da rede de mineração."""
import contextlib
import io
import json
import threading
import time
import unittest
from http.server import BaseHTTPRequestHandler, ThreadingHTTPServer
from unittest.mock import patch

import minerador
from protocolo import hash_bloco, preparar_hash, prova_valida


class TestProtocolo(unittest.TestCase):
    def test_hash_otimizado_preserva_formato(self):
        trabalhos = [
            {"id": "a", "altura": 1, "dados": "teste", "dificuldade": 2},
            {"dificuldade": 0, "dados": {"z": [1, None, True], "a": "ação ☕"}, "id": 2},
            {"dados": "aspas: \" e barra: \\ e quebra: \n", "dificuldade": 64},
        ]
        for trabalho in trabalhos:
            for nome in ("Aluno", 'Máquina "A" \\ ☕'):
                calcular = preparar_hash(trabalho, nome)
                for nonce in (0, 9, 10, 2**63, 2**64 - 1):
                    with self.subTest(trabalho=trabalho, nome=nome, nonce=nonce):
                        self.assertEqual(calcular(nonce), hash_bloco(trabalho, nome, nonce))

    def test_trabalho_invalido_nao_chega_aos_processos(self):
        for estado in ({}, [], {"trabalho": {}}, {"trabalho": {
            "id": 1, "altura": 1, "dados": "teste", "dificuldade": -1,
        }}):
            with self.subTest(estado=estado), self.assertRaises(ValueError):
                minerador.validar_trabalho(estado)
        self.assertIsNone(minerador.validar_trabalho({"trabalho": None}))


class TestProcessos(unittest.TestCase):
    def aguardar(self, mineracao, quantidade):
        prazo = time.monotonic() + 10
        solucoes = []
        while len(solucoes) < quantidade and time.monotonic() < prazo:
            solucoes.extend(mineracao.consultar())
            if len(solucoes) < quantidade:
                minerador.wait(mineracao.conexoes, timeout=0.02)
        self.assertEqual(len(solucoes), quantidade)
        return solucoes

    def test_processos_dividem_nonces_e_trocam_rodada(self):
        mineracao = minerador.MineracaoParalela(2)
        trabalho = {"id": 1, "altura": 1, "dados": "teste", "dificuldade": 0}
        try:
            with patch("minerador.random.randrange", return_value=100):
                mineracao.iniciar(trabalho, "Aluno")
            solucoes = self.aguardar(mineracao, 2)
            self.assertEqual({nonce for nonce, _ in solucoes}, {100, 101})
            for nonce, digest in solucoes:
                self.assertEqual(digest, hash_bloco(trabalho, "Aluno", nonce))

            # Produz resultados antigos e troca a rodada antes de consumi-los.
            mineracao.iniciar(trabalho, "Aluno")
            for conexao in mineracao.conexoes:
                self.assertTrue(conexao.poll(10))
            novo = dict(trabalho, id=2, dados="nova rodada")
            mineracao.parar()
            mineracao.iniciar(novo, "Aluno")
            for nonce, digest in self.aguardar(mineracao, 2):
                self.assertEqual(digest, hash_bloco(novo, "Aluno", nonce))
        finally:
            mineracao.fechar()
        self.assertTrue(all(not processo.is_alive() for processo in mineracao.processos))

    def test_cancelamento_interrompe_pausa_longa(self):
        mineracao = minerador.MineracaoParalela(1, pausa=60)
        trabalho = {"id": 1, "altura": 1, "dados": "teste", "dificuldade": 64}
        try:
            mineracao.iniciar(trabalho, "Aluno")
            # Dá tempo ao processo de começar a executar o trabalho difícil.
            time.sleep(0.3)
            novo = dict(trabalho, id=2, dificuldade=0)
            mineracao.parar()
            mineracao.iniciar(novo, "Aluno")
            nonce, digest = self.aguardar(mineracao, 1)[0]
            self.assertEqual(digest, hash_bloco(novo, "Aluno", nonce))
        finally:
            mineracao.fechar()


class TestIntegracao(unittest.TestCase):
    def test_http_continua_durante_mineracao_e_envia_prova(self):
        for motor in ("python", "auto", "nativo-evp") if minerador.selecionar_motor() != "python" else ("python",):
            with self.subTest(motor=motor):
                self.exercitar_http(motor)

    def exercitar_http(self, motor):
        trabalho = {"id": "ocupado", "altura": 1, "dados": "ação", "dificuldade": 64}
        seguinte = dict(trabalho, id="solucionavel", altura=2, dificuldade=1)
        estado = {"consultas": 0, "presencas": 0, "solucoes": []}
        aceita = threading.Event()

        class Servidor(BaseHTTPRequestHandler):
            def log_message(self, *args):
                pass

            def responder(self, codigo, dados):
                corpo = json.dumps(dados).encode()
                self.send_response(codigo)
                self.send_header("Content-Type", "application/json")
                self.send_header("Content-Length", str(len(corpo)))
                self.end_headers()
                self.wfile.write(corpo)

            def do_GET(self):
                if self.path != "/estado":
                    self.responder(404, {})
                    return
                estado["consultas"] += 1
                atual = trabalho if estado["consultas"] <= 2 else seguinte
                self.responder(200, {"trabalho": None if aceita.is_set() else atual})

            def do_POST(self):
                dados = json.loads(self.rfile.read(int(self.headers["Content-Length"])))
                if self.path == "/presenca":
                    estado["presencas"] += 1
                    self.responder(200, {})
                elif self.path == "/solucao":
                    estado["solucoes"].append(dados)
                    valida = dados["id"] == seguinte["id"] and prova_valida(
                        seguinte, dados["minerador"], dados["nonce"]
                    )
                    self.responder(201 if valida else 400, {"valida": valida})
                    if valida:
                        aceita.set()
                else:
                    self.responder(404, {})

        servidor = ThreadingHTTPServer(("127.0.0.1", 0), Servidor)
        thread = threading.Thread(target=servidor.serve_forever, daemon=True)
        thread.start()
        prazo = time.monotonic() + 15
        esperar = minerador.wait
        requisitar = minerador.requisicao

        def requisicao_com_prazo(*args, **kwargs):
            if time.monotonic() > prazo:
                raise AssertionError("Prazo do teste HTTP excedido; verifique o acesso ao servidor local.")
            return requisitar(*args, **kwargs)

        def aguardar_ou_encerrar(*args, **kwargs):
            if aceita.is_set():
                raise KeyboardInterrupt
            if time.monotonic() > prazo:
                raise AssertionError("Minerador não enviou uma solução no prazo.")
            return esperar(*args, **kwargs)

        argumentos = [
            "minerador.py", "--servidor", f"http://127.0.0.1:{servidor.server_port}",
            "--nome", "Aluno", "--processos", "1" if motor == "cuda" else "2", "--motor", motor,
        ]
        saida = io.StringIO()
        try:
            with patch("sys.argv", argumentos), patch(
                "minerador.wait", side_effect=aguardar_ou_encerrar
            ), patch(
                "minerador.requisicao", side_effect=requisicao_com_prazo
            ), contextlib.redirect_stdout(saida):
                self.assertEqual(minerador.main(), 0)
        finally:
            servidor.shutdown()
            servidor.server_close()
            thread.join(timeout=2)
        self.assertTrue(aceita.is_set())
        self.assertGreaterEqual(estado["consultas"], 3)
        self.assertGreaterEqual(estado["presencas"], 1)
        self.assertEqual(estado["solucoes"][0]["minerador"], "Aluno")
        self.assertIn("Bloco aceito!", saida.getvalue())
        self.assertIn("Minerador encerrado.", saida.getvalue())


if __name__ == "__main__":
    unittest.main()
