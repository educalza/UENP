"""Cliente minerador: usa processos paralelos e acompanha as rodadas."""
import argparse
import json
import multiprocessing
import os
import random
import signal
import time
from multiprocessing.connection import wait
from urllib.error import HTTPError, URLError
from urllib.request import Request, urlopen

from motores import criar_motor, selecionar_motor

INTERVALO_PROGRESSO = 0.5


def requisicao(servidor, rota, dados=None):
    corpo = None if dados is None else json.dumps(dados).encode()
    req = Request(servidor + rota, data=corpo, headers={"Content-Type": "application/json"})
    try:
        with urlopen(req, timeout=5) as resposta:
            return resposta.status, json.load(resposta)
    except HTTPError as erro:
        return erro.code, json.load(erro)


def cpus_disponiveis():
    contar = getattr(os, "process_cpu_count", os.cpu_count)
    return contar() or 1


def minerar(conexao, indice, quantidade, pausa, escolha_motor):
    """Calcula sem fazer HTTP; recebe novos trabalhos entre pequenos lotes."""
    signal.signal(signal.SIGINT, signal.SIG_IGN)
    motor = None
    # Um múltiplo do passo mantém os trabalhadores em sequências distintas,
    # inclusive quando o contador de 64 bits dá a volta.
    limite_nonce = (2**64 // quantidade) * quantidade
    try:
        conexao.send(("pronto", 0, 0))
        while True:
            comando = conexao.recv() if motor is None else None
            if comando is None and conexao.poll():
                comando = conexao.recv()
            while comando is not None:
                if comando[0] == "encerrar":
                    return
                if comando[0] == "parar":
                    if motor is not None:
                        motor.fechar()
                        motor = None
                elif comando[0] == "trabalho":
                    _, geracao, trabalho, nome, inicio_nonce, duracao = comando
                    if motor is not None:
                        motor.fechar()
                    motor = criar_motor(trabalho, nome, escolha_motor)
                    nonce = (inicio_nonce + indice) % limite_nonce
                    tentativas = 0
                    ultimo_progresso = time.monotonic()
                    fim_aquecimento = None
                    inicio_medicao = None
                comando = conexao.recv() if conexao.poll() else None
            if motor is None:
                continue
            nonce, contagem, solucao = motor.buscar(nonce, quantidade, limite_nonce)
            tentativas += contagem
            if solucao is not None:
                conexao.send(("solucao", geracao, tentativas, *solucao))
                motor.fechar()
                motor = None
            if motor is not None:
                agora = time.monotonic()
                if duracao is not None:
                    if fim_aquecimento is None:
                        # A preparação do primeiro lote não conta como
                        # aquecimento do cálculo.
                        fim_aquecimento = agora + 0.3
                        tentativas = 0
                    elif inicio_medicao is None and agora >= fim_aquecimento:
                        inicio_medicao = agora
                        tentativas = 0
                    elif inicio_medicao is not None and agora - inicio_medicao >= duracao:
                        conexao.send(("medicao", geracao, tentativas, agora - inicio_medicao))
                        motor.fechar()
                        motor = None
                elif agora - ultimo_progresso >= INTERVALO_PROGRESSO:
                    conexao.send(("progresso", geracao, tentativas))
                    ultimo_progresso = agora
                if pausa and motor is not None:
                    # Uma nova rodada interrompe a pausa imediatamente.
                    conexao.poll(pausa)
    except (EOFError, OSError):
        pass
    finally:
        if motor is not None:
            motor.fechar()
        conexao.close()


class MineracaoParalela:
    def __init__(self, quantidade, pausa=0, motor="auto"):
        self.processos = []
        self.conexoes = []
        self.contagens = [0] * quantidade
        self.medicoes = [None] * quantidade
        self.geracao = 0
        self.trabalho = None
        self.inicio = time.monotonic()
        self.encerrado = False
        escolha_motor = selecionar_motor(motor)
        contexto = multiprocessing.get_context("spawn")
        try:
            for indice in range(quantidade):
                pai, filho = contexto.Pipe()
                processo = contexto.Process(
                    target=minerar,
                    args=(filho, indice, quantidade, pausa, escolha_motor),
                    daemon=True,
                )
                try:
                    processo.start()
                except BaseException:
                    pai.close()
                    raise
                finally:
                    filho.close()
                self.processos.append(processo)
                self.conexoes.append(pai)
            prazo = time.monotonic() + 30
            for conexao in self.conexoes:
                if not conexao.poll(max(0, prazo - time.monotonic())):
                    raise RuntimeError("Tempo limite ao iniciar os processos de mineração.")
                try:
                    mensagem = conexao.recv()
                except (EOFError, OSError) as erro:
                    raise RuntimeError("Um processo falhou durante a inicialização.") from erro
                if mensagem[0] != "pronto":
                    raise RuntimeError("Resposta inesperada durante a inicialização.")
        except BaseException:
            self.fechar()
            raise

    def iniciar(self, trabalho, nome, duracao=None):
        self.geracao += 1
        self.trabalho = trabalho
        self.contagens = [0] * len(self.processos)
        self.medicoes = [None] * len(self.processos)
        self.inicio = time.monotonic()
        inicio_nonce = random.randrange(2**63)
        for conexao in self.conexoes:
            conexao.send(("trabalho", self.geracao, trabalho, nome, inicio_nonce, duracao))

    def parar(self):
        self.geracao += 1
        self.trabalho = None
        for conexao in self.conexoes:
            conexao.send(("parar",))

    def consultar(self):
        solucoes = []
        for indice, (processo, conexao) in enumerate(zip(self.processos, self.conexoes)):
            if not processo.is_alive():
                raise RuntimeError(f"Processo de mineração {indice + 1} encerrou inesperadamente.")
            try:
                while conexao.poll():
                    mensagem = conexao.recv()
                    if mensagem[1] != self.geracao:
                        continue
                    self.contagens[indice] = mensagem[2]
                    if mensagem[0] == "solucao":
                        solucoes.append((mensagem[3], mensagem[4]))
                    elif mensagem[0] == "medicao":
                        self.medicoes[indice] = mensagem[2] / mensagem[3]
            except (EOFError, OSError) as erro:
                raise RuntimeError("Perdida a comunicação com um processo de mineração.") from erro
        return solucoes

    def fechar(self):
        if self.encerrado:
            return
        self.encerrado = True
        for conexao in self.conexoes:
            try:
                conexao.send(("encerrar",))
            except OSError:
                pass
        prazo = time.monotonic() + 3
        for processo in self.processos:
            processo.join(timeout=max(0, prazo - time.monotonic()))
        for processo in self.processos:
            if processo.is_alive():
                processo.terminate()
        for processo in self.processos:
            processo.join(timeout=1)
        for conexao in self.conexoes:
            conexao.close()


def validar_trabalho(estado):
    if not isinstance(estado, dict) or "trabalho" not in estado:
        raise ValueError("Estado sem o campo trabalho.")
    trabalho = estado["trabalho"]
    if trabalho is not None:
        campos = ("id", "altura", "dados", "dificuldade")
        if not isinstance(trabalho, dict) or any(campo not in trabalho for campo in campos):
            raise ValueError("Trabalho incompleto.")
        dificuldade = trabalho["dificuldade"]
        if type(dificuldade) is not int or not 0 <= dificuldade <= 64:
            raise ValueError("Dificuldade deve ser um inteiro entre 0 e 64.")
    return trabalho


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--servidor", help="Ex.: http://172.16.3.30:8001")
    parser.add_argument("--nome", help="Nome único do aluno ou da máquina")
    parser.add_argument(
        "--processos", type=int,
        help="Processos de cálculo; padrão: melhor benchmark salvo ou todas as CPUs disponíveis.",
    )
    parser.add_argument(
        "--pausa", type=float, default=0,
        help="Descanso entre lotes por processo, em segundos; padrão: 0 (máximo desempenho).",
    )
    parser.add_argument("--motor", choices=('auto', 'nativo', 'nativo-evp', 'python'), default="auto",
                        help="CPU: usa C se disponível; Python como alternativa.")
    parser.add_argument("--benchmark", action="store_true",
                        help="Mede o desempenho local e salva a melhor quantidade de processos.")
    parser.add_argument("--duracao", type=float, default=1.5,
                        help="Segundos de medição por configuração do benchmark; padrão: 1.5.")
    parser.add_argument("--consulta", type=float, default=0.25,
                        help="Intervalo de consulta de novas rodadas em segundos; padrão: 0.25.")
    args = parser.parse_args()
    if not 0 <= args.pausa <= 60:
        parser.error("Use pausa entre 0 e 60 segundos.")
    if args.processos is not None and args.processos < 1:
        parser.error("Use pelo menos 1 processo.")
    if not 0.2 <= args.duracao <= 30:
        parser.error("Use duração de benchmark entre 0.2 e 30 segundos.")
    if not 0.1 <= args.consulta <= 60:
        parser.error("Use intervalo de consulta entre 0.1 e 60 segundos.")
    if not args.benchmark and (not args.servidor or not args.nome):
        parser.error("Informe --servidor e --nome, ou use --benchmark para um teste local.")
    if args.nome is not None and (not args.nome.strip() or len(args.nome) > 60):
        parser.error("Use nome de 1 a 60 caracteres e pausa entre 0 e 60 segundos.")
    ultima_consulta = ultimo_relatorio = ultima_presenca = 0
    mineracao = None
    try:
        from desempenho import carregar_configuracao, executar_benchmark

        motor = selecionar_motor(args.motor)
        if args.benchmark:
            executar_benchmark(MineracaoParalela, motor, cpus_disponiveis(),
                               args.duracao, args.processos)
            return 0
        quantidade = args.processos or carregar_configuracao(motor) or cpus_disponiveis()
        servidor = args.servidor.rstrip("/")
        mineracao = MineracaoParalela(quantidade, args.pausa, motor)
        print(f"Minerador iniciado com {quantidade} processos | motor {motor}. Ctrl+C para encerrar.")
        while True:
            try:
                solucoes = mineracao.consultar()
                if solucoes and mineracao.trabalho is not None:
                    trabalho = mineracao.trabalho
                    nonce, digest = solucoes[0]
                    mineracao.parar()
                    print(f"Prova encontrada: {digest}")
                    codigo, resultado = requisicao(
                        servidor, "/solucao",
                        {"id": trabalho["id"], "minerador": args.nome, "nonce": nonce},
                    )
                    print("Bloco aceito!" if codigo == 201 else f"Recusado: {resultado}")
                    ultima_consulta = 0
                agora = time.monotonic()
                if agora - ultima_presenca >= 5:
                    requisicao(servidor, "/presenca", {"minerador": args.nome})
                    ultima_presenca = time.monotonic()
                if agora - ultima_consulta >= args.consulta:
                    codigo, estado = requisicao(servidor, "/estado")
                    if codigo != 200:
                        raise ValueError(f"Resposta do servidor: {estado}")
                    trabalho = validar_trabalho(estado)
                    ultima_consulta = time.monotonic()
                    if trabalho is None:
                        if mineracao.trabalho is not None:
                            mineracao.parar()
                    elif trabalho != mineracao.trabalho:
                        mineracao.iniciar(trabalho, args.nome)
                        ultimo_relatorio = mineracao.inicio
                        print(
                            f"Minerando bloco {trabalho['altura']} | "
                            f"dificuldade {trabalho['dificuldade']} | dados: {trabalho['dados']}"
                        )
                agora = time.monotonic()
                if mineracao.trabalho is not None and agora - ultimo_relatorio >= 3:
                    tentativas = sum(mineracao.contagens)
                    segundos = max(agora - mineracao.inicio, 0.001)
                    print(f"{tentativas:,} tentativas | {tentativas / segundos:,.0f} hashes/s (total)")
                    ultimo_relatorio = agora
                wait(mineracao.conexoes, timeout=0.02)
            except (URLError, OSError, ValueError) as erro:
                print(f"Falha na conexão ou resposta do servidor: {erro}. Nova tentativa em 2 segundos.")
                mineracao.parar()
                ultima_consulta = ultima_presenca = 0
                time.sleep(2)
    except KeyboardInterrupt:
        print("\nMinerador encerrado.")
    except RuntimeError as erro:
        print(f"Falha no minerador: {erro}")
        return 1
    finally:
        if mineracao is not None:
            mineracao.fechar()
    return 0


if __name__ == "__main__":
    multiprocessing.freeze_support()
    raise SystemExit(main())
