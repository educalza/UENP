"""Formato de hash compartilhado pelo servidor e pelos mineradores."""
import hashlib
import json


def hash_bloco(trabalho, minerador, nonce):
    dados = {"trabalho": trabalho, "minerador": minerador, "nonce": nonce}
    serializado = json.dumps(dados, sort_keys=True, separators=(",", ":"), ensure_ascii=False).encode()
    return hashlib.sha256(hashlib.sha256(serializado).digest()).hexdigest()


def partes_bloco(trabalho, minerador):
    """Partes fixas do JSON canônico; somente o nonce muda entre tentativas."""
    prefixo = ('{"minerador":' + json.dumps(minerador, ensure_ascii=False) + ',"nonce":').encode()
    sufixo = (',"trabalho":' + json.dumps(
        trabalho, sort_keys=True, separators=(",", ":"), ensure_ascii=False
    ) + '}').encode()
    return prefixo, sufixo


def preparar_hash_bytes(trabalho, minerador):
    prefixo, sufixo = partes_bloco(trabalho, minerador)
    sha256 = hashlib.sha256

    def calcular(nonce):
        serializado = prefixo + str(nonce).encode("ascii") + sufixo
        return sha256(sha256(serializado).digest()).digest()

    return calcular


def preparar_hash(trabalho, minerador):
    """Serializa as partes fixas uma vez, preservando o formato de hash_bloco."""
    prefixo, sufixo = partes_bloco(trabalho, minerador)
    sha256 = hashlib.sha256

    def calcular(nonce):
        serializado = prefixo + str(nonce).encode("ascii") + sufixo
        return sha256(sha256(serializado).digest()).hexdigest()

    return calcular


def prova_valida(trabalho, minerador, nonce):
    return hash_bloco(trabalho, minerador, nonce).startswith("0" * trabalho["dificuldade"])
