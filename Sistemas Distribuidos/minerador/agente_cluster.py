"""Agente CPU/CUDA para um PC Windows ou Linux do cluster local."""
import argparse
import json
import multiprocessing
from pathlib import Path
import platform
import secrets
import socket
import time

from cluster.nucleo import chamar_http, url_http
from minerador import MineracaoParalela, cpus_disponiveis
from motores import selecionar_motor


class Agente:
    def __init__(self, config, http=chamar_http):
        self.config = config
        self.url = url_http(config["coordenador"])
        self.http = http
        self.sessao = secrets.token_hex(16)
        self.pool = None
        self.perfil = None
        self.job = None
        self.chave = None
        self.tentativas = self.contagem = self.sequencia = 0
        self.ultimo = time.monotonic()
        self.velocidade = 0
        self.estado = "aguardando"
        self.erro = ""
        self.gpu = ""
        self.ultima_falha = None
        self.repetir_motor = 0
        self.processos = 0
        self.motor = ""
        self.mensagem = None

    def requisicao(self, rota, dados):
        return self.http(self.url, rota, {"id": self.config["id"], "sessao": self.sessao, **dados},
                         token=self.config["token"])

    def coletar(self):
        if self.pool is None or self.job is None:
            return []
        solucoes = self.pool.consultar()
        total = sum(self.pool.contagens)
        delta = max(0, total - self.contagem)
        self.tentativas += delta
        self.contagem = total
        agora = time.monotonic()
        if agora - self.ultimo >= 0.4:
            self.velocidade = delta / max(agora - self.ultimo, 0.001)
            self.ultimo = agora
        return solucoes

    def parar(self, fechar=False):
        if self.pool:
            if fechar:
                self.pool.fechar()
                self.pool = None
                self.perfil = None
            elif self.job:
                self.pool.parar()
        self.job = self.chave = None
        self.contagem = 0
        self.velocidade = 0

    def aplicar(self, resposta):
        config = resposta["config"]
        job = resposta["trabalho"]
        if job is None:
            self.parar()
            self.estado = "aguardando" if config["habilitado"] else "pausado"
            self.erro = ""
            return
        desejado = (config["motor"], config["processos"])
        if self.ultima_falha == desejado and time.monotonic() < self.repetir_motor:
            return
        try:
            motor = selecionar_motor(config["motor"])
            processos = 1 if motor == "cuda" else (config["processos"] or cpus_disponiveis())
            processos = min(processos, cpus_disponiveis())
            perfil = (motor, processos)
            if perfil != self.perfil:
                self.parar(fechar=True)
                if motor == "cuda":
                    from cuda_minerador import informacoes_cuda
                    self.gpu = informacoes_cuda()["nome"]
                self.pool = MineracaoParalela(processos, pausa=0, motor=motor)
                self.perfil = perfil
            self.motor, self.processos = perfil
            chave = (job["geracao"], perfil, config["slot"])
            if chave != self.chave:
                self.pool.iniciar(job["bloco"], job["minerador"], inicio_nonce=int(job["inicio_nonce"]),
                                  particao=config["slot"], particoes=job["particoes"])
                self.job = job
                self.chave = chave
                self.contagem = 0
                self.ultimo = time.monotonic()
                self.velocidade = 0
            self.estado, self.erro = "minerando", ""
            self.ultima_falha = None
        except (OSError, ValueError, RuntimeError) as erro:
            self.parar(fechar=True)
            self.estado, self.erro = "erro", str(erro)
            self.ultima_falha = desejado
            self.repetir_motor = time.monotonic() + 15

    def ciclo(self):
        try:
            solucoes = self.coletar()
        except (OSError, RuntimeError) as erro:
            self.parar(fechar=True)
            self.estado, self.erro = "erro", str(erro)
            solucoes = []
        self.sequencia += 1
        relato = {"host": socket.gethostname(), "sistema": platform.system(), "cpus": cpus_disponiveis(),
                  "gpu": self.gpu, "motor": self.motor, "processos": self.processos,
                  "estado": self.estado, "erro": self.erro, "velocidade": self.velocidade,
                  "job": self.job["geracao"] if self.job else ""}
        codigo, resposta = self.requisicao("/api/agente/sincronizar", {
            "sequencia": self.sequencia, "tentativas": self.tentativas, "relato": relato})
        if codigo != 200:
            raise RuntimeError(resposta.get("erro", f"HTTP {codigo}"))
        # Antes de enviar uma prova, confirme que o coordenador mantém a rodada.
        if solucoes and self.job and resposta["trabalho"] and resposta["trabalho"]["geracao"] == self.job["geracao"]:
            geracao = self.job["geracao"]
            self.parar()
            codigo, resultado = self.requisicao("/api/agente/solucao", {
                "geracao": geracao, "nonce": str(solucoes[0][0])})
            print("Bloco aceito pela fonte!" if codigo == 200 and resultado.get("aceito")
                  else f"Resultado da prova: {resultado}", flush=True)
            self.estado = "aguardando"
            return 0.1
        self.aplicar(resposta)
        return 0.5

    def executar(self):
        try:
            while True:
                try:
                    atraso = self.ciclo()
                except (OSError, ValueError, RuntimeError) as erro:
                    self.parar()
                    self.estado, self.erro = "desconectado", str(erro)
                    atraso = 2
                mensagem = (self.estado, self.erro, self.motor, self.processos)
                if mensagem != self.mensagem:
                    print(f"{self.config['id']}: {self.estado} | {self.motor} | {self.processos} processos"
                          + (f" | {self.erro}" if self.erro else ""), flush=True)
                    self.mensagem = mensagem
                time.sleep(atraso)
        except KeyboardInterrupt:
            print("\nAgente encerrado.")
        finally:
            self.parar(fechar=True)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--config", required=True, help="Arquivo pc-01.json, etc., baixado no painel.")
    parser.add_argument("--coordenador", help="Substitui o endereço do arquivo, ex.: http://172.16.3.50:8080")
    args = parser.parse_args()
    try:
        config = json.loads(Path(args.config).read_text(encoding="utf-8-sig"))
        if args.coordenador:
            config["coordenador"] = args.coordenador
        if config.get("id") not in {f"pc-{i:02d}" for i in range(1, 7)} or not isinstance(config.get("token"), str):
            raise ValueError("Arquivo do agente inválido. Baixe a configuração no painel.")
        agente = Agente(config)
        print(f"Agente {config['id']} → {agente.url}. Ctrl+C para encerrar.", flush=True)
        agente.executar()
    except (OSError, ValueError, KeyError) as erro:
        print(f"Falha: {erro}")
        return 1
    return 0


if __name__ == "__main__":
    multiprocessing.freeze_support()
    raise SystemExit(main())
