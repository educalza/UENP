"""Instala somente o compilador CUDA NVRTC na pasta local do projeto."""
from pathlib import Path
import subprocess
import sys


def main():
    destino = Path(__file__).resolve().parent / ".cuda-deps"
    subprocess.run([
        sys.executable, "-m", "pip", "install", "--target", str(destino),
        "--upgrade", "--no-deps", "--only-binary=:all:",
        "nvidia-cuda-nvrtc-cu12==12.9.86",
    ], check=True)
    from cuda_minerador import informacoes_cuda
    info = informacoes_cuda()
    print(f"CUDA pronto: {info['nome']} | compute {info['arquitetura']}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
