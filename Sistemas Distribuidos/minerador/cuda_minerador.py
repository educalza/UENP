"""CUDA C compilado com NVRTC e executado pelo driver NVIDIA, sem CUDA Toolkit."""
import atexit
import ctypes as ct
from ctypes.util import find_library
from functools import lru_cache
import hashlib
import os
from pathlib import Path
import re
import struct
import sys

from protocolo import hash_bloco, partes_bloco

RAIZ = Path(__file__).resolve().parent
FONTE = RAIZ / "native" / "minerador_cuda.cu"
INICIAL = (0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a,
           0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19)
MASCARA = 0xffffffff


def declarar(biblioteca, nome, argumentos, retorno=ct.c_int):
    funcao = getattr(biblioteca, nome)
    funcao.argtypes = argumentos
    funcao.restype = retorno
    return funcao


def conferir(driver, codigo):
    if codigo:
        mensagem = ct.c_char_p()
        driver.cuGetErrorString(codigo, ct.byref(mensagem))
        texto = mensagem.value.decode(errors="replace") if mensagem.value else str(codigo)
        raise RuntimeError(f"CUDA: {texto} (código {codigo}).")


@lru_cache(maxsize=1)
def carregar_driver():
    try:
        driver = ct.WinDLL("nvcuda.dll") if os.name == "nt" else ct.CDLL("libcuda.so.1")
    except OSError as erro:
        raise RuntimeError("Driver CUDA NVIDIA não encontrado.") from erro
    ponteiro = ct.c_void_p
    declarar(driver, "cuGetErrorString", [ct.c_int, ct.POINTER(ct.c_char_p)])
    declarar(driver, "cuInit", [ct.c_uint])
    declarar(driver, "cuDriverGetVersion", [ct.POINTER(ct.c_int)])
    declarar(driver, "cuDeviceGetCount", [ct.POINTER(ct.c_int)])
    declarar(driver, "cuDeviceGet", [ct.POINTER(ct.c_int), ct.c_int])
    declarar(driver, "cuDeviceGetName", [ct.c_char_p, ct.c_int, ct.c_int])
    declarar(driver, "cuDeviceGetAttribute", [ct.POINTER(ct.c_int), ct.c_int, ct.c_int])
    declarar(driver, "cuCtxCreate_v2", [ct.POINTER(ponteiro), ct.c_uint, ct.c_int])
    declarar(driver, "cuCtxSetCurrent", [ponteiro])
    declarar(driver, "cuCtxDestroy_v2", [ponteiro])
    declarar(driver, "cuCtxSynchronize", [])
    declarar(driver, "cuMemAlloc_v2", [ct.POINTER(ct.c_uint64), ct.c_size_t])
    declarar(driver, "cuMemFree_v2", [ct.c_uint64])
    declarar(driver, "cuMemcpyHtoD_v2", [ct.c_uint64, ponteiro, ct.c_size_t])
    declarar(driver, "cuMemcpyDtoH_v2", [ponteiro, ct.c_uint64, ct.c_size_t])
    declarar(driver, "cuModuleLoadData", [ct.POINTER(ponteiro), ponteiro])
    declarar(driver, "cuModuleGetFunction", [ct.POINTER(ponteiro), ponteiro, ct.c_char_p])
    declarar(driver, "cuModuleUnload", [ponteiro])
    declarar(driver, "cuLaunchKernel", [ponteiro] + [ct.c_uint] * 7 +
             [ponteiro, ct.POINTER(ponteiro), ct.POINTER(ponteiro)])
    conferir(driver, driver.cuInit(0))
    numero = ct.c_int()
    conferir(driver, driver.cuDeviceGetCount(ct.byref(numero)))
    if numero.value < 1:
        raise RuntimeError("Nenhuma GPU CUDA disponível.")
    return driver


@lru_cache(maxsize=1)
def carregar_nvrtc():
    candidatos = []
    if os.environ.get("MINERADOR_NVRTC"):
        candidatos.append(Path(os.environ["MINERADOR_NVRTC"]))
    local = RAIZ / ".cuda-deps" / "nvidia" / "cuda_nvrtc"
    if os.name == "nt":
        candidatos.extend(sorted(local.glob("bin/nvrtc64*.dll")))
        if os.environ.get("CUDA_PATH"):
            candidatos.extend(sorted((Path(os.environ["CUDA_PATH"]) / "bin").glob("nvrtc64*.dll")))
    else:
        candidatos.extend(sorted(local.glob("lib/libnvrtc.so*")))
        encontrado = find_library("nvrtc")
        if encontrado:
            candidatos.append(Path(encontrado))
    for caminho in candidatos:
        if ".alt." in caminho.name:
            continue
        try:
            diretorio = os.add_dll_directory(str(caminho.parent)) if os.name == "nt" else None
            if os.name == "nt":
                builtins = sorted(caminho.parent.glob("nvrtc-builtins64*.dll"))
                if builtins:
                    # NVRTC faz seu próprio LoadLibrary pelo nome da DLL.
                    # Carregá-la por caminho absoluto também funciona fora do PATH.
                    runtime_builtins = ct.CDLL(str(builtins[0]))
            biblioteca = ct.CDLL(str(caminho))
            if os.name == "nt" and builtins:
                biblioteca._runtime_builtins = runtime_builtins
            break
        except OSError:
            continue
    else:
        raise RuntimeError("Compilador CUDA NVRTC ausente. Execute: python preparar_cuda.py")
    ptr = ct.c_void_p
    declarar(biblioteca, "nvrtcVersion", [ct.POINTER(ct.c_int), ct.POINTER(ct.c_int)])
    declarar(biblioteca, "nvrtcCreateProgram", [ct.POINTER(ptr), ct.c_char_p, ct.c_char_p,
             ct.c_int, ct.POINTER(ct.c_char_p), ct.POINTER(ct.c_char_p)])
    declarar(biblioteca, "nvrtcCompileProgram", [ptr, ct.c_int, ct.POINTER(ct.c_char_p)])
    declarar(biblioteca, "nvrtcGetProgramLogSize", [ptr, ct.POINTER(ct.c_size_t)])
    declarar(biblioteca, "nvrtcGetProgramLog", [ptr, ct.c_void_p])
    declarar(biblioteca, "nvrtcGetPTXSize", [ptr, ct.POINTER(ct.c_size_t)])
    declarar(biblioteca, "nvrtcGetPTX", [ptr, ct.c_void_p])
    declarar(biblioteca, "nvrtcDestroyProgram", [ct.POINTER(ptr)])
    return biblioteca, diretorio


def informacoes_cuda():
    driver = carregar_driver()
    carregar_nvrtc()
    dispositivo = ct.c_int()
    conferir(driver, driver.cuDeviceGet(ct.byref(dispositivo), 0))
    nome = ct.create_string_buffer(256)
    conferir(driver, driver.cuDeviceGetName(nome, len(nome), dispositivo))
    major, minor, versao = ct.c_int(), ct.c_int(), ct.c_int()
    conferir(driver, driver.cuDeviceGetAttribute(ct.byref(major), 75, dispositivo))
    conferir(driver, driver.cuDeviceGetAttribute(ct.byref(minor), 76, dispositivo))
    conferir(driver, driver.cuDriverGetVersion(ct.byref(versao)))
    return {"nome": nome.value.decode(), "arquitetura": f"{major.value}{minor.value}",
            "driver": versao.value, "dispositivo": dispositivo.value}


def compilar_ptx(fonte, arquitetura):
    nvrtc, _ = carregar_nvrtc()
    major, minor = ct.c_int(), ct.c_int()
    if nvrtc.nvrtcVersion(ct.byref(major), ct.byref(minor)):
        raise RuntimeError("Falha ao identificar NVRTC.")
    chave = hashlib.sha256(f"{arquitetura}:{major.value}:{minor.value}:".encode() + fonte).hexdigest()
    pasta = RAIZ / "native" / "cuda_cache"
    arquivo = pasta / f"{chave}.ptx"
    if arquivo.exists():
        return arquivo.read_bytes()
    programa = ct.c_void_p()
    if nvrtc.nvrtcCreateProgram(ct.byref(programa), fonte, b"minerador_cuda.cu", 0, None, None):
        raise RuntimeError("Falha ao criar programa CUDA.")
    try:
        opcoes = (ct.c_char_p * 2)(f"--gpu-architecture=compute_{arquitetura}".encode(), b"--std=c++14")
        codigo = nvrtc.nvrtcCompileProgram(programa, len(opcoes), opcoes)
        tamanho = ct.c_size_t()
        if codigo:
            nvrtc.nvrtcGetProgramLogSize(programa, ct.byref(tamanho))
            log = ct.create_string_buffer(max(tamanho.value, 1))
            nvrtc.nvrtcGetProgramLog(programa, log)
            raise RuntimeError(f"Falha ao compilar CUDA: {log.value.decode(errors='replace')}")
        if nvrtc.nvrtcGetPTXSize(programa, ct.byref(tamanho)):
            raise RuntimeError("Falha ao obter tamanho do programa CUDA.")
        ptx = ct.create_string_buffer(tamanho.value)
        if nvrtc.nvrtcGetPTX(programa, ptx):
            raise RuntimeError("Falha ao obter programa CUDA.")
        dados = ptx.raw
        pasta.mkdir(exist_ok=True)
        temporario = arquivo.with_suffix(f".{os.getpid()}.tmp")
        try:
            temporario.write_bytes(dados)
            temporario.replace(arquivo)
        finally:
            if temporario.exists():
                temporario.unlink()
        return dados
    finally:
        nvrtc.nvrtcDestroyProgram(ct.byref(programa))


class SessaoCUDA:
    def __init__(self):
        self.driver = carregar_driver()
        self.info = informacoes_cuda()
        self.contexto = ct.c_void_p()
        self.modulos = {}
        # BLOCKING_SYNC evita ocupar uma CPU inteira esperando a GPU.
        conferir(self.driver, self.driver.cuCtxCreate_v2(ct.byref(self.contexto), 4, self.info["dispositivo"]))

    def ativar(self):
        conferir(self.driver, self.driver.cuCtxSetCurrent(self.contexto))

    def kernel(self, fonte):
        self.ativar()
        chave = hashlib.sha256(fonte).digest()
        if chave not in self.modulos:
            ptx = compilar_ptx(fonte, self.info["arquitetura"])
            modulo, funcao = ct.c_void_p(), ct.c_void_p()
            conferir(self.driver, self.driver.cuModuleLoadData(ct.byref(modulo), ct.c_char_p(ptx)))
            try:
                conferir(self.driver, self.driver.cuModuleGetFunction(ct.byref(funcao), modulo, b"buscar"))
            except BaseException:
                self.driver.cuModuleUnload(modulo)
                raise
            self.modulos[chave] = modulo, funcao
        return self.modulos[chave][1]

    def fechar(self):
        if self.contexto:
            self.driver.cuCtxSetCurrent(self.contexto)
            for modulo, _ in self.modulos.values():
                self.driver.cuModuleUnload(modulo)
            self.modulos.clear()
            self.driver.cuCtxDestroy_v2(self.contexto)
            self.contexto = ct.c_void_p()


_sessao = None


def sessao_cuda():
    global _sessao
    if _sessao is None or not _sessao.contexto:
        _sessao = SessaoCUDA()
    return _sessao


def fechar_sessao():
    if _sessao is not None:
        _sessao.fechar()


atexit.register(fechar_sessao)


def estado_prefixo(prefixo, fonte):
    """Pré-calcula os blocos completos anteriores ao nonce uma vez por trabalho."""
    constantes = tuple(int(x, 16) for x in re.findall(r"0x[0-9a-fA-F]+", fonte.split("K[64] = {", 1)[1].split("};", 1)[0]))
    if len(constantes) != 64:
        raise RuntimeError("Constantes SHA-256 do kernel inválidas.")
    estado = list(INICIAL)
    rot = lambda x, n: ((x >> n) | (x << (32 - n))) & MASCARA
    for pos in range(0, len(prefixo) // 64 * 64, 64):
        w = list(struct.unpack(">16I", prefixo[pos:pos+64]))
        for i in range(16, 64):
            x, y = w[i-15], w[i-2]
            w.append((w[i-16] + (rot(x, 7) ^ rot(x, 18) ^ (x >> 3)) + w[i-7]
                      + (rot(y, 17) ^ rot(y, 19) ^ (y >> 10))) & MASCARA)
        a, b, c, d, e, f, g, h = estado
        for k, palavra in zip(constantes, w):
            t1 = (h + (rot(e, 6) ^ rot(e, 11) ^ rot(e, 25)) + ((e & f) ^ (~e & g)) + k + palavra) & MASCARA
            t2 = ((rot(a, 2) ^ rot(a, 13) ^ rot(a, 22)) + ((a & b) ^ (a & c) ^ (b & c))) & MASCARA
            a, b, c, d, e, f, g, h = (t1+t2) & MASCARA, a, b, c, (d+t1) & MASCARA, e, f, g
        estado = [(x+y) & MASCARA for x, y in zip(estado, (a,b,c,d,e,f,g,h))]
    return estado


class MotorCUDA:
    nome = "cuda"
    lote = 262144

    def __init__(self, trabalho, nome):
        self.sessao = sessao_cuda()
        self.driver = self.sessao.driver
        self.trabalho, self.minerador = trabalho, nome
        self.dificuldade = trabalho["dificuldade"]
        self.prefixo, self.sufixo = partes_bloco(trabalho, nome)
        blocos = (len(self.prefixo) % 64 + 20 + len(self.sufixo) + 72) // 64
        self.lote = max(128, min(type(self).lote, type(self).lote * 4 // max(4, blocos)))
        self.fonte = FONTE.read_text(encoding="utf-8")
        self.estado = estado_prefixo(self.prefixo, self.fonte)
        self.programas = {}
        self.saida_gpu = ct.c_uint64()
        self.saida = (ct.c_uint32 * 9)()
        self.fechado = False
        self.sessao.ativar()
        conferir(self.driver, self.driver.cuMemAlloc_v2(ct.byref(self.saida_gpu), ct.sizeof(self.saida)))

    def programa(self, digitos):
        if digitos not in self.programas:
            mensagem = self.prefixo + b"\0" * digitos + self.sufixo
            tamanho = len(mensagem)
            mensagem += b"\x80" + b"\0" * ((55 - tamanho) % 64) + (tamanho * 8).to_bytes(8, "big")
            ignorar = len(self.prefixo) // 64 * 64
            mensagem = mensagem[ignorar:]
            palavras = struct.unpack(f">{len(mensagem)//4}I", mensagem)
            defines = {"DIGITOS": digitos, "OFFSET_NONCE": len(self.prefixo) % 64,
                       "BLOCOS": len(mensagem) // 64}
            defines.update({f"ESTADO_{i}": f"0x{x:08x}u" for i, x in enumerate(self.estado)})
            fonte = ("\n".join(f"#define {k} {v}" for k, v in defines.items()) + "\n" + self.fonte).encode()
            kernel = self.sessao.kernel(fonte)
            modelo = (ct.c_uint32 * len(palavras))(*palavras)
            ponteiro = ct.c_uint64()
            conferir(self.driver, self.driver.cuMemAlloc_v2(ct.byref(ponteiro), ct.sizeof(modelo)))
            try:
                conferir(self.driver, self.driver.cuMemcpyHtoD_v2(ponteiro, modelo, ct.sizeof(modelo)))
            except BaseException:
                self.driver.cuMemFree_v2(ponteiro)
                raise
            self.programas[digitos] = kernel, ponteiro
        return self.programas[digitos]

    def buscar(self, nonce, passo, limite, lote=None):
        if self.fechado:
            raise RuntimeError("Motor CUDA já encerrado.")
        lote = self.lote if lote is None else lote
        if not (0 <= nonce < limite <= 2**64 and 0 < passo <= limite and 0 <= lote < 2**32):
            raise ValueError("Faixa de nonces ou tamanho de lote CUDA inválido.")
        self.sessao.ativar()
        tentativas = 0
        while tentativas < lote:
            digitos = len(str(nonce))
            fim = min(limite, 10**digitos)
            quantidade = min(lote - tentativas, (fim - 1 - nonce) // passo + 1, self.lote)
            kernel, modelo = self.programa(digitos)
            vazio = ct.c_uint32(MASCARA)
            conferir(self.driver, self.driver.cuMemcpyHtoD_v2(self.saida_gpu, ct.byref(vazio), 4))
            valores = (ct.c_uint64(modelo.value), ct.c_uint64(nonce), ct.c_uint64(passo),
                       ct.c_uint32(quantidade), ct.c_uint32(self.dificuldade), ct.c_uint64(self.saida_gpu.value))
            parametros = (ct.c_void_p * len(valores))(*(ct.cast(ct.byref(v), ct.c_void_p) for v in valores))
            conferir(self.driver, self.driver.cuLaunchKernel(kernel, (quantidade+127)//128, 1, 1,
                                                           128, 1, 1, 0, None, parametros, None))
            conferir(self.driver, self.driver.cuCtxSynchronize())
            conferir(self.driver, self.driver.cuMemcpyDtoH_v2(self.saida, self.saida_gpu, ct.sizeof(self.saida)))
            # A primeira thread serve de verificação contínua do algoritmo na GPU.
            digest_primeiro = struct.pack(">8I", *self.saida[1:]).hex()
            if digest_primeiro != hash_bloco(self.trabalho, self.minerador, nonce):
                raise RuntimeError("Hash CUDA divergiu do protocolo original.")
            tentativas += quantidade
            proximo = (nonce + quantidade * passo) % limite
            if self.saida[0] != MASCARA:
                encontrado = nonce + self.saida[0] * passo
                digest = hash_bloco(self.trabalho, self.minerador, encontrado)
                if not digest.startswith("0" * self.dificuldade):
                    raise RuntimeError("Prova CUDA inválida ao conferir na CPU.")
                return proximo, tentativas, (encontrado, digest)
            nonce = proximo
        return nonce, tentativas, None

    def fechar(self):
        if not self.fechado:
            self.sessao.ativar()
            for _, modelo in self.programas.values():
                self.driver.cuMemFree_v2(modelo)
            self.programas.clear()
            self.driver.cuMemFree_v2(self.saida_gpu)
            self.fechado = True
