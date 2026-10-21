"""Benchmark sem rede, com aquecimento e escolha da quantidade de processos."""
import hashlib
import json
import os
from pathlib import Path
import platform
import sys
import time
from multiprocessing.connection import wait

from motores import caminho_biblioteca

RAIZ = Path(__file__).resolve().parent
ARQUIVO = RAIZ / "desempenho.json"
ARQUIVO_CUDA = RAIZ / "desempenho_cuda.json"
TRABALHO = {
    "id": "benchmark-local", "altura": 1,
    "dados": "Teste local de desempenho, sem acesso à rede.", "dificuldade": 64,
}


def identidade(motor):
    arquivos = [RAIZ / nome for nome in ("minerador.py", "motores.py", "protocolo.py")]
    if motor.startswith("nativo"):
        arquivos.append(caminho_biblioteca())
    dados = {
        "versao": 1, "motor": motor, "python": sys.version,
        "maquina": platform.machine(), "processador": platform.processor(),
        "cpus": getattr(os, "process_cpu_count", os.cpu_count)(),
        "codigo": [hashlib.sha256(p.read_bytes()).hexdigest() for p in arquivos],
    }
    if motor == "cuda":
        from cuda_minerador import informacoes_cuda
        dados["gpu"] = informacoes_cuda()
        dados["cuda"] = [hashlib.sha256((RAIZ / nome).read_bytes()).hexdigest()
                         for nome in ("cuda_minerador.py", "native/minerador_cuda.cu")]
    return dados


def carregar_configuracao(motor):
    try:
        arquivo = ARQUIVO_CUDA if motor == "cuda" else ARQUIVO
        dados = json.loads(arquivo.read_text(encoding="utf-8"))
        quantidade = dados["processos"]
        if dados["identidade"] == identidade(motor) and type(quantidade) is int and quantidade > 0:
            return quantidade
    except (OSError, ValueError, KeyError, TypeError):
        pass
    return None


def medir(criar_pool, quantidade, motor, duracao):
    pool = criar_pool(quantidade, motor=motor)
    try:
        pool.iniciar(TRABALHO, "Benchmark", duracao=duracao)
        prazo = time.monotonic() + duracao + 30
        while any(taxa is None for taxa in pool.medicoes):
            pool.consultar()
            if time.monotonic() > prazo:
                raise RuntimeError("Tempo limite ao medir os processos de mineração.")
            wait(pool.conexoes, timeout=0.02)
        return sum(pool.medicoes)
    finally:
        pool.fechar()


def executar_benchmark(criar_pool, motor, cpus, duracao=1.5, quantidade=None):
    candidatos = sorted({1, max(1, cpus // 4), max(1, cpus // 2), max(1, cpus * 3 // 4), cpus})
    if motor == "cuda":
        if quantidade not in (None, 1):
            raise ValueError("O benchmark CUDA usa um processo para a GPU.")
        candidatos = [1]
    if quantidade is not None:
        candidatos = [quantidade]
    print(f"Benchmark local | motor {motor} | {duracao:g}s por configuração", flush=True)
    resultados = []
    for processos in candidatos:
        taxa = medir(criar_pool, processos, motor, duracao)
        resultados.append({"processos": processos, "hashes_s": taxa})
        print(f"  {processos:2} processos: {taxa:,.0f} hashes/s", flush=True)
    melhor = max(resultados, key=lambda linha: linha["hashes_s"])
    dados = {"identidade": identidade(motor), "processos": melhor["processos"],
             "resultados": resultados, "duracao": duracao}
    arquivo = ARQUIVO_CUDA if motor == "cuda" else ARQUIVO
    temporario = arquivo.with_suffix(".tmp")
    try:
        temporario.write_text(json.dumps(dados, ensure_ascii=False, indent=2) + "\n", encoding="utf-8")
        temporario.replace(arquivo)
    finally:
        if temporario.exists():
            temporario.unlink()
    print(f"Melhor resultado salvo: {melhor['processos']} processos, "
          f"{melhor['hashes_s']:,.0f} hashes/s. Será usado nas próximas execuções.", flush=True)
    return dados
