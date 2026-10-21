"""Gera o pacote portátil dos agentes e do painel, sem chaves nem caches locais."""
from pathlib import Path
import zipfile


def main():
    raiz = Path(__file__).resolve().parent
    arquivos = ["coordenador.py", "agente_cluster.py", "minerador.py", "motores.py", "protocolo.py",
                "cuda_minerador.py", "compilar.py", "preparar_cuda.py", "desempenho.py", "README.md",
                "CLUSTER.md", "empacotar_cluster.py", "test_cluster.py", "test_minerador.py",
                "test_motores.py", "test_cuda.py", "native/minerador.c", "native/minerador_cuda.cu"]
    arquivos += [p.relative_to(raiz).as_posix() for p in sorted((raiz / "cluster").rglob("*"))
                 if p.is_file() and p.suffix in (".py", ".html", ".css", ".js")]
    # Os fontes permitem recompilar em Linux; a DLL existente atende Windows x64.
    dll = "native/minerador_native_v2.dll"
    if (raiz / dll).exists():
        arquivos.append(dll)
    destino = raiz / "cluster_minerador.zip"
    with zipfile.ZipFile(destino, "w", zipfile.ZIP_DEFLATED, compresslevel=9) as pacote:
        for arquivo in arquivos:
            pacote.write(raiz / arquivo, "cluster_minerador/" + arquivo)
    with zipfile.ZipFile(destino) as pacote:
        if pacote.testzip():
            raise RuntimeError("Falha na integridade do pacote.")
    print(f"Pacote pronto: {destino} | {len(arquivos)} arquivos | {destino.stat().st_size:,} bytes")


if __name__ == "__main__":
    main()
