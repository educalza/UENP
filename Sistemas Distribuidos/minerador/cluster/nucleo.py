"""Estado do cluster e comunicação com a fonte, sem dependências externas."""
from collections import deque
import copy
import hashlib
import json
import math
from pathlib import Path
import secrets
import threading
import time
from urllib.error import HTTPError
from urllib.parse import urlsplit
from urllib.request import ProxyHandler, Request, build_opener

from minerador import validar_trabalho
from protocolo import hash_bloco

MOTORES = ("auto", "nativo", "nativo-evp", "python", "cuda")
ONLINE = 10


class ErroAPI(Exception):
    def __init__(self, codigo, mensagem):
        super().__init__(mensagem)
        self.codigo = codigo


def url_http(valor):
    if not isinstance(valor, str) or len(valor) > 300:
        raise ValueError("Informe um endereço HTTP válido.")
    url = urlsplit(valor)
    if url.scheme not in ("http", "https") or not url.hostname or url.username or url.password or url.query or url.fragment:
        raise ValueError("Use um endereço http://IP:porta, sem usuário ou parâmetros.")
    _ = url.port
    return valor.rstrip("/")


def chamar_http(base, rota, dados=None, token=None, timeout=4):
    corpo = None if dados is None else json.dumps(dados, ensure_ascii=False, allow_nan=False).encode()
    cabecalhos = {"Content-Type": "application/json"}
    if token:
        cabecalhos["Authorization"] = "Bearer " + token
    req = Request(base + rota, data=corpo, headers=cabecalhos)
    # A comunicação da LAN não deve passar pelo proxy do sistema.
    try:
        resposta = build_opener(ProxyHandler({})).open(req, timeout=timeout)
    except HTTPError as erro:
        resposta = erro
    with resposta:
        bruto = resposta.read(2_000_001)
        if len(bruto) > 2_000_000:
            raise ValueError("Resposta HTTP excede o limite de 2 MB.")
        try:
            conteudo = json.loads(bruto) if bruto else {}
        except (ValueError, UnicodeError):
            conteudo = {"erro": bruto[:200].decode(errors="replace")}
        return resposta.code, conteudo


def assinatura(trabalho):
    return hashlib.sha256(json.dumps(trabalho, sort_keys=True, ensure_ascii=False,
                                    separators=(",", ":"), allow_nan=False).encode()).hexdigest()


class Coordenador:
    def __init__(self, arquivo, fonte="http://172.16.3.30:8001", nome="Cluster", http=chamar_http):
        self.arquivo = Path(arquivo)
        self.lock = threading.RLock()
        self.http = http
        self.fim = threading.Event()
        self.thread = None
        self.boot = secrets.token_hex(8)
        self.revisao = 0
        self.contador = 0
        self.ativo = False
        self.fonte_online = False
        self.erro_fonte = "Cluster pausado."
        self.trabalho = None
        self.job = None
        self.eventos = deque(maxlen=80)
        self.historico = deque(maxlen=90)
        self.finalizados = deque(maxlen=256)
        self.enviando = set()
        self.aceitos = self.recusados = self.obsoletos = 0
        self.config = {
            "fonte": url_http(fonte), "nome": nome,
            "token_admin": secrets.token_urlsafe(24), "token_agente": secrets.token_urlsafe(32),
            "nos": [{"id": f"pc-{i+1:02d}", "nome": f"PC {i+1:02d}", "slot": i,
                     "motor": "auto" if i < 2 else "cuda", "processos": 0, "habilitado": True}
                    for i in range(6)],
        }
        if self.arquivo.exists():
            salvo = json.loads(self.arquivo.read_text(encoding="utf-8"))
            # O inventário fixa seis slots para impedir colisões ao reconectar.
            if not isinstance(salvo, dict) or len(salvo.get("nos", [])) != 6:
                raise ValueError("Configuração do cluster inválida: são necessários seis PCs.")
            for i, no in enumerate(salvo["nos"]):
                if no.get("slot") != i or no.get("id") != f"pc-{i+1:02d}":
                    raise ValueError("IDs ou partições inválidos no arquivo do cluster.")
                self.validar_no(no)
            self.validar_config(salvo)
            for chave in ("token_admin", "token_agente"):
                if not isinstance(salvo.get(chave), str) or len(salvo[chave]) < 24:
                    raise ValueError("Chave de acesso inválida no arquivo do cluster.")
            self.config = salvo
        else:
            self.validar_config(self.config)
            self.salvar()
        self.nos = {n["id"]: {"sessao": None, "visto": 0, "relato": {}, "total": 0,
                              "contador": 0, "sequencia": -1} for n in self.config["nos"]}
        self.evento("Coordenador pronto. Conecte os agentes e inicie o cluster.")

    def salvar(self):
        self.arquivo.parent.mkdir(parents=True, exist_ok=True)
        temporario = self.arquivo.with_suffix(".tmp")
        temporario.write_text(json.dumps(self.config, ensure_ascii=False, indent=2), encoding="utf-8")
        temporario.replace(self.arquivo)
        try:
            self.arquivo.chmod(0o600)
        except OSError:
            pass

    def evento(self, texto):
        self.eventos.appendleft({"hora": time.time(), "texto": texto})

    @staticmethod
    def validar_config(dados):
        url_http(dados.get("fonte"))
        nome = dados.get("nome")
        if not isinstance(nome, str) or not nome.strip() or len(nome) > 60:
            raise ValueError("Nome na fonte deve ter entre 1 e 60 caracteres.")

    @staticmethod
    def validar_no(dados):
        if dados.get("motor") not in MOTORES:
            raise ValueError("Motor inválido.")
        if type(dados.get("processos")) is not int or not 0 <= dados["processos"] <= 1024:
            raise ValueError("Processos deve ser 0 (automático) ou de 1 a 1024.")
        if type(dados.get("habilitado")) is not bool:
            raise ValueError("Estado do PC inválido.")
        if not isinstance(dados.get("nome"), str) or not dados["nome"].strip() or len(dados["nome"]) > 60:
            raise ValueError("Nome do PC deve ter entre 1 e 60 caracteres.")

    def invalidar(self):
        self.revisao += 1
        self.trabalho = self.job = None

    def configurar(self, dados):
        with self.lock:
            alterado = dict(self.config)
            alterado.update({k: dados[k] for k in ("fonte", "nome") if k in dados})
            self.validar_config(alterado)
            alterado["fonte"] = url_http(alterado["fonte"])
            self.config = alterado
            self.salvar()
            self.invalidar()
            self.finalizados.clear()
            self.fonte_online = False
            self.erro_fonte = "Aguardando consulta à fonte."
            self.evento("Configuração da fonte atualizada.")

    def controlar(self, acao):
        if acao not in ("iniciar", "pausar"):
            raise ValueError("Ação inválida.")
        with self.lock:
            self.ativo = acao == "iniciar"
            self.invalidar()
            self.fonte_online = False
            self.erro_fonte = "Aguardando consulta à fonte." if self.ativo else "Cluster pausado."
            self.evento("Cluster iniciado." if self.ativo else "Cluster pausado.")

    def configurar_no(self, identidade, dados):
        with self.lock:
            no = next((n for n in self.config["nos"] if n["id"] == identidade), None)
            if no is None:
                raise ErroAPI(404, "PC não encontrado.")
            alterado = dict(no)
            alterado.update({k: dados[k] for k in ("nome", "motor", "processos", "habilitado") if k in dados})
            self.validar_no(alterado)
            no.update(alterado)
            self.salvar()
            self.evento(f"{no['nome']}: configuração atualizada.")

    def _sessao(self, dados):
        identidade, sessao = dados.get("id"), dados.get("sessao")
        if not isinstance(identidade, str) or identidade not in self.nos:
            raise ErroAPI(404, "ID desconhecido. Baixe a configuração deste PC no painel.")
        if not isinstance(sessao, str) or not 16 <= len(sessao) <= 80:
            raise ErroAPI(400, "Sessão do agente inválida.")
        no = self.nos[identidade]
        if no["sessao"] != sessao:
            if no["sessao"] is not None and time.monotonic() - no["visto"] < ONLINE:
                raise ErroAPI(409, "Este ID já está conectado. Use um arquivo diferente para cada PC.")
            no.update(sessao=sessao, contador=0, sequencia=-1)
            self.evento(f"{identidade} conectado.")
        return no

    def sincronizar(self, dados):
        with self.lock:
            no = self._sessao(dados)
            sequencia, contador = dados.get("sequencia"), dados.get("tentativas")
            if type(sequencia) is not int or sequencia < 0 or type(contador) is not int or contador < 0:
                raise ErroAPI(400, "Contadores inválidos.")
            # Heartbeats retransmitidos não devem duplicar a contagem.
            if sequencia > no["sequencia"]:
                relato = dados.get("relato", {})
                if not isinstance(relato, dict):
                    raise ErroAPI(400, "Relato inválido.")
                velocidade = relato.get("velocidade", 0)
                if not isinstance(velocidade, (int, float)) or not math.isfinite(velocidade) or velocidade < 0:
                    raise ErroAPI(400, "Velocidade inválida.")
                if contador < no["contador"]:
                    raise ErroAPI(400, "O contador da sessão não pode diminuir.")
                no["total"] += contador - no["contador"]
                no["contador"] = contador
                no["sequencia"] = sequencia
                no["relato"] = {k: str(relato.get(k, ""))[:300] for k in
                                ("host", "sistema", "gpu", "motor", "estado", "erro", "job")}
                no["relato"].update(velocidade=velocidade, cpus=relato.get("cpus", 0),
                                   processos=relato.get("processos", 0))
            no["visto"] = time.monotonic()
            config_no = next(n for n in self.config["nos"] if n["id"] == dados["id"])
            return {"config": dict(config_no), "trabalho": copy.deepcopy(self.job)
                    if self.ativo and config_no["habilitado"] else None, "intervalo": 0.5}

    def atualizar_fonte(self, trabalho):
        """Chamado sob lock após validar uma resposta da revisão atual."""
        self.fonte_online = True
        self.erro_fonte = ""
        if trabalho is None or assinatura(trabalho) in self.finalizados:
            self.trabalho = self.job = None
        elif trabalho != self.trabalho:
            self.contador += 1
            self.trabalho = copy.deepcopy(trabalho)
            self.job = {"geracao": f"{self.boot}:{self.contador}", "bloco": copy.deepcopy(trabalho),
                        "minerador": self.config["nome"], "inicio_nonce": str(secrets.randbelow(2**63)),
                        "particoes": 6}
            self.evento(f"Novo trabalho: bloco {trabalho['altura']}, dificuldade {trabalho['dificuldade']}.")

    def solucao(self, dados):
        with self.lock:
            no = self._sessao(dados)
            config_no = next(n for n in self.config["nos"] if n["id"] == dados["id"])
            job = self.job
            if not self.ativo or not config_no["habilitado"] or job is None or dados.get("geracao") != job["geracao"]:
                self.obsoletos += 1
                raise ErroAPI(409, "Rodada encerrada ou PC pausado.")
            try:
                texto = dados["nonce"]
                if not isinstance(texto, str) or not texto.isascii() or not texto.isdigit() or len(texto) > 20:
                    raise ValueError()
                nonce = int(texto)
                if not 0 <= nonce < 2**64 or (nonce - int(job["inicio_nonce"])) % 6 != config_no["slot"]:
                    raise ValueError()
            except (KeyError, ValueError):
                raise ErroAPI(400, "Nonce fora da partição deste PC.") from None
            digest = hash_bloco(job["bloco"], job["minerador"], nonce)
            if not digest.startswith("0" * job["bloco"]["dificuldade"]):
                raise ErroAPI(400, "Prova de trabalho inválida.")
            chave = job["geracao"]
            if chave in self.enviando:
                raise ErroAPI(409, "Já existe uma prova em envio para esta rodada.")
            self.enviando.add(chave)
            fonte = self.config["fonte"]
            revisao = self.revisao
        try:
            codigo, resposta = self.http(fonte, "/solucao", {"id": job["bloco"]["id"],
                                        "minerador": job["minerador"], "nonce": nonce})
        except (OSError, ValueError) as erro:
            with self.lock:
                self.enviando.discard(chave)
                if self.revisao == revisao and self.job == job:
                    self.falha_fonte(erro)
            raise ErroAPI(502, "A fonte não confirmou a solução; aguardando reconexão.") from erro
        with self.lock:
            self.enviando.discard(chave)
            if codigo == 201:
                self.aceitos += 1
                self.finalizados.append(assinatura(job["bloco"]))
                self.evento(f"Bloco {job['bloco']['altura']} aceito! Encontrado por {config_no['nome']}.")
            else:
                self.recusados += 1
                self.evento(f"Fonte recusou a prova de {config_no['nome']} (HTTP {codigo}).")
            if self.job == job:
                self.trabalho = self.job = None
            return {"aceito": codigo == 201, "codigo_fonte": codigo, "resposta": resposta}

    def falha_fonte(self, erro):
        texto = str(erro)[:300]
        if self.fonte_online or texto != self.erro_fonte:
            self.evento("Falha na fonte: " + texto)
        self.fonte_online = False
        self.erro_fonte = texto
        self.trabalho = self.job = None

    def ciclo_fonte(self):
        presenca = 0
        revisao_anterior = -1
        while not self.fim.is_set():
            with self.lock:
                ativo, revisao = self.ativo, self.revisao
                fonte, nome = self.config["fonte"], self.config["nome"]
                self.historico.append({"hora": time.time(), "velocidade": self.velocidade()})
            atraso = 0.5
            if ativo:
                try:
                    if revisao != revisao_anterior or time.monotonic() - presenca >= 5:
                        codigo, _ = self.http(fonte, "/presenca", {"minerador": nome})
                        if not 200 <= codigo < 300:
                            raise ValueError(f"Presença recusada pela fonte: HTTP {codigo}.")
                        presenca = time.monotonic()
                    codigo, estado = self.http(fonte, "/estado")
                    if codigo != 200:
                        raise ValueError(f"Estado da fonte: HTTP {codigo}.")
                    trabalho = validar_trabalho(estado)
                    with self.lock:
                        if revisao == self.revisao and self.ativo:
                            self.atualizar_fonte(trabalho)
                    revisao_anterior = revisao
                except (OSError, ValueError) as erro:
                    with self.lock:
                        if revisao == self.revisao and self.ativo:
                            self.falha_fonte(erro)
                    presenca = 0
                    atraso = 2
            self.fim.wait(atraso)

    def velocidade(self):
        return sum(n["relato"].get("velocidade", 0) for identidade, n in self.nos.items()
                   if self.minerando(identidade))

    def minerando(self, identidade):
        no = self.nos[identidade]
        config = next(n for n in self.config["nos"] if n["id"] == identidade)
        return (self.ativo and config["habilitado"] and self.job is not None
                and time.monotonic() - no["visto"] < ONLINE
                and no["relato"].get("estado") == "minerando"
                and no["relato"].get("job") == self.job["geracao"])

    def estado(self):
        with self.lock:
            agora = time.monotonic()
            nos = []
            for config in self.config["nos"]:
                no = self.nos[config["id"]]
                online = no["sessao"] is not None and agora - no["visto"] < ONLINE
                nos.append({**config, "online": online, "minerando": self.minerando(config["id"]),
                            "segundos_sem_contato": round(agora - no["visto"], 1) if no["sessao"] else None,
                            "relato": dict(no["relato"]), "tentativas": str(no["total"])})
            return {"ativo": self.ativo, "fonte": self.config["fonte"], "nome": self.config["nome"],
                    "fonte_online": self.fonte_online, "erro_fonte": self.erro_fonte,
                    "trabalho": copy.deepcopy(self.trabalho), "geracao": self.job["geracao"] if self.job else None,
                    "velocidade": self.velocidade(), "tentativas": str(sum(n["total"] for n in self.nos.values())),
                    "aceitos": self.aceitos, "recusados": self.recusados, "obsoletos": self.obsoletos,
                    "nos": nos, "eventos": list(self.eventos), "historico": list(self.historico)}

    def iniciar(self):
        self.thread = threading.Thread(target=self.ciclo_fonte, name="fonte", daemon=True)
        self.thread.start()

    def fechar(self):
        self.fim.set()
        if self.thread:
            self.thread.join(timeout=9)
