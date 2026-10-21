"""Lotes de mineração em Python ou C, com o mesmo protocolo e resultado."""
import ctypes
from ctypes.util import find_library
from functools import lru_cache
from pathlib import Path
import sys

from compilar import nome_biblioteca
from protocolo import hash_bloco, partes_bloco, preparar_hash_bytes


class Funcoes(ctypes.Structure):
    _fields_ = [(nome, ctypes.c_void_p) for nome in (
        "ctx_new", "ctx_free", "init", "update", "final", "sha256",
        "direto_init", "direto_update", "direto_final",
    )]


class Resultado(ctypes.Structure):
    _fields_ = [
        ("tentativas", ctypes.c_uint64),
        ("proximo_nonce", ctypes.c_uint64),
        ("nonce", ctypes.c_uint64),
        ("digest", ctypes.c_ubyte * 32),
    ]


def caminho_biblioteca():
    return Path(__file__).resolve().parent / "native" / nome_biblioteca()


@lru_cache(maxsize=1)
def carregar_nativo():
    biblioteca = ctypes.CDLL(str(caminho_biblioteca()))
    biblioteca.minerador_abi.restype = ctypes.c_int
    if biblioteca.minerador_abi() != 2:
        raise RuntimeError("Motor nativo incompatível; execute compilar.py novamente.")
    candidatos = []
    if sys.platform == "win32":
        for pasta in (Path(sys.base_prefix) / "DLLs", Path(sys.base_prefix)):
            candidatos.extend(str(p) for p in sorted(pasta.glob("libcrypto*.dll")))
    if not candidatos:
        encontrado = find_library("crypto")
        if encontrado:
            candidatos.append(encontrado)
    simbolos = ("EVP_MD_CTX_new", "EVP_MD_CTX_free", "EVP_DigestInit_ex",
                "EVP_DigestUpdate", "EVP_DigestFinal_ex", "EVP_sha256")
    for caminho in candidatos:
        try:
            crypto = ctypes.CDLL(caminho)
            funcoes = Funcoes(*(ctypes.cast(getattr(crypto, nome), ctypes.c_void_p).value
                                for nome in simbolos), None, None, None)
            break
        except (OSError, AttributeError):
            continue
    else:
        raise OSError("Biblioteca OpenSSL com SHA-256 não encontrada.")
    biblioteca.minerador_openssl_major.restype = ctypes.c_ulong
    try:
        crypto.OpenSSL_version_num.restype = ctypes.c_ulong
        versao_compilada = biblioteca.minerador_openssl_major()
        if versao_compilada and versao_compilada == crypto.OpenSSL_version_num() >> 28:
            diretas = [ctypes.cast(getattr(crypto, nome), ctypes.c_void_p).value
                       for nome in ("SHA256_Init", "SHA256_Update", "SHA256_Final")]
            funcoes.direto_init, funcoes.direto_update, funcoes.direto_final = diretas
    except AttributeError:
        pass
    biblioteca.minerador_criar.argtypes = [
        ctypes.POINTER(Funcoes), ctypes.c_char_p, ctypes.c_size_t,
        ctypes.c_char_p, ctypes.c_size_t, ctypes.c_uint,
    ]
    biblioteca.minerador_criar.restype = ctypes.c_void_p
    biblioteca.minerador_destruir.argtypes = [ctypes.c_void_p]
    biblioteca.minerador_destruir.restype = None
    biblioteca.minerador_buscar.argtypes = [
        ctypes.c_void_p, ctypes.c_uint64, ctypes.c_uint64, ctypes.c_uint64,
        ctypes.c_uint32, ctypes.c_uint, ctypes.POINTER(Resultado),
    ]
    biblioteca.minerador_buscar.restype = ctypes.c_int
    # Mantém o OpenSSL carregado enquanto existirem ponteiros usados pelo C.
    return biblioteca, crypto, funcoes


class MotorPython:
    nome = "python"
    lote = 2048

    def __init__(self, trabalho, nome):
        self.calcular = preparar_hash_bytes(trabalho, nome)
        self.zeros = b"\0" * (trabalho["dificuldade"] // 2)
        self.impar = trabalho["dificuldade"] % 2

    def buscar(self, nonce, passo, limite, lote=None):
        lote = self.lote if lote is None else lote
        calcular, zeros, impar = self.calcular, self.zeros, self.impar
        completos = len(zeros)
        for indice in range(lote):
            digest = calcular(nonce)
            proximo = (nonce + passo) % limite
            if digest.startswith(zeros) and (not impar or digest[completos] < 16):
                return proximo, indice + 1, (nonce, digest.hex())
            nonce = proximo
        return nonce, lote, None

    def fechar(self):
        pass


class MotorNativo:
    nome = "nativo"
    lote = 8192

    def __init__(self, trabalho, nome, variante="auto"):
        self.biblioteca, self.crypto, self.funcoes = carregar_nativo()
        self.dificuldade = trabalho["dificuldade"]
        direto = variante != "evp" and all((self.funcoes.direto_init,
                                           self.funcoes.direto_update, self.funcoes.direto_final))
        self.nome = "nativo" if direto else "nativo-evp"
        prefixo, sufixo = partes_bloco(trabalho, nome)
        self.contexto = self.biblioteca.minerador_criar(
            ctypes.byref(self.funcoes), prefixo, len(prefixo), sufixo, len(sufixo), direto
        )
        if not self.contexto:
            raise RuntimeError("Não foi possível iniciar o cálculo nativo.")
        self.resultado = Resultado()
        try:
            # Confere o protocolo antes de iniciar qualquer lote de mineração.
            status = self.biblioteca.minerador_buscar(
                self.contexto, 0, 1, 0, 1, 0, ctypes.byref(self.resultado)
            )
            if status != 1 or bytes(self.resultado.digest).hex() != hash_bloco(trabalho, nome, 0):
                raise RuntimeError("Motor nativo divergiu do protocolo de hash.")
        except BaseException:
            self.fechar()
            raise

    def buscar(self, nonce, passo, limite, lote=None):
        status = self.biblioteca.minerador_buscar(
            self.contexto, nonce, passo, limite % 2**64,
            self.lote if lote is None else lote, self.dificuldade, ctypes.byref(self.resultado),
        )
        if status < 0:
            raise RuntimeError("Falha ao calcular lote nativo.")
        solucao = None
        if status:
            solucao = (self.resultado.nonce, bytes(self.resultado.digest).hex())
        return self.resultado.proximo_nonce, self.resultado.tentativas, solucao

    def fechar(self):
        if self.contexto:
            self.biblioteca.minerador_destruir(self.contexto)
            self.contexto = None


def criar_motor(trabalho, nome, escolha="auto"):
    if escolha not in ("auto", "nativo", "nativo-evp", "python"):
        raise ValueError("Escolha um motor de CPU: auto, nativo, nativo-evp ou python.")
    if escolha != "python":
        try:
            return MotorNativo(trabalho, nome, "evp" if escolha == "nativo-evp" else "auto")
        except (OSError, AttributeError, RuntimeError) as erro:
            if escolha in ("nativo", "nativo-evp"):
                raise RuntimeError(f"Motor nativo indisponível: {erro} Execute compilar.py.") from erro
    return MotorPython(trabalho, nome)


def selecionar_motor(escolha="auto"):
    motor = criar_motor({"id": "verificacao", "dificuldade": 0}, "Teste", escolha)
    try:
        return motor.nome
    finally:
        motor.fechar()
