"""Compila o motor nativo com GCC/Clang, sem instalar pacotes ou bibliotecas."""
import argparse
import os
from pathlib import Path
import shutil
import subprocess
import sys


def nome_biblioteca():
    if sys.platform == "win32":
        return "minerador_native_v2.dll"
    if sys.platform == "darwin":
        return "libminerador_native_v2.dylib"
    return "libminerador_native_v2.so"


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--somente-evp", action="store_true",
                        help="Compila sem headers OpenSSL, mantendo apenas a via EVP.")
    args = parser.parse_args()
    raiz = Path(__file__).resolve().parent
    compilador = os.environ.get("CC") or shutil.which("gcc") or shutil.which("clang")
    if not compilador:
        print("Instale GCC ou Clang para compilar. O motor Python continua disponível.")
        return 1
    destino = raiz / "native" / nome_biblioteca()
    temporario = destino.with_name(destino.stem + ".tmp" + destino.suffix)
    comando = [compilador, "-O3", "-flto", "-std=c11", "-Wall", "-Wextra", "-Werror"]
    comando += ["-dynamiclib"] if sys.platform == "darwin" else ["-shared"]
    if os.name != "nt":
        comando.append("-fPIC")
    if os.environ.get("OPENSSL_INCLUDE_DIR"):
        comando += ["-I", os.environ["OPENSSL_INCLUDE_DIR"]]
    if args.somente_evp:
        comando.append("-DMINERADOR_SEM_SHA_DIRETO")
    comando += [str(raiz / "native" / "minerador.c"), "-o", str(temporario)]
    try:
        resultado = subprocess.run(comando, capture_output=True, text=True)
        if resultado.returncode and not args.somente_evp:
            # A via EVP continua compilável onde não há headers do OpenSSL.
            print("Via SHA direta indisponível; compilando com a via EVP.")
            comando.insert(1, "-DMINERADOR_SEM_SHA_DIRETO")
            resultado = subprocess.run(comando, capture_output=True, text=True)
        if resultado.returncode:
            print(resultado.stderr, file=sys.stderr)
            return resultado.returncode
        temporario.replace(destino)
    finally:
        if temporario.exists():
            temporario.unlink()
    print(f"Motor nativo compilado: {destino}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
