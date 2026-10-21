"""Servidor HTTP do painel e da API dos agentes da rede local."""
import argparse
import hmac
from http.server import BaseHTTPRequestHandler, ThreadingHTTPServer
import json
import mimetypes
from pathlib import Path
import socket
from urllib.parse import urlsplit

from cluster.nucleo import Coordenador, ErroAPI, url_http

WEB = Path(__file__).resolve().parent / "web" / "dist"


class Servidor(ThreadingHTTPServer):
    daemon_threads = True
    allow_reuse_address = True

    def __init__(self, endereco, coordenador):
        self.coordenador = coordenador
        super().__init__(endereco, Handler)


class Handler(BaseHTTPRequestHandler):
    server_version = "ClusterLocal/1.0"

    def log_message(self, formato, *args):
        if args and str(args[1] if len(args) > 1 else "") not in ("200", "201"):
            super().log_message(formato, *args)

    def enviar(self, codigo, dados, tipo="application/json; charset=utf-8", arquivo=None):
        corpo = dados if isinstance(dados, bytes) else json.dumps(dados, ensure_ascii=False, allow_nan=False).encode()
        self.send_response(codigo)
        self.send_header("Content-Type", tipo)
        self.send_header("Content-Length", str(len(corpo)))
        self.send_header("Cache-Control", "no-store")
        self.send_header("X-Content-Type-Options", "nosniff")
        self.send_header("Referrer-Policy", "no-referrer")
        self.send_header("Content-Security-Policy", "default-src 'self'; style-src 'self'; script-src 'self'; connect-src 'self'; img-src 'self' data:; object-src 'none'; frame-ancestors 'none'; base-uri 'none'")
        if arquivo:
            self.send_header("Content-Disposition", f'attachment; filename="{arquivo}"')
        self.end_headers()
        self.wfile.write(corpo)

    def autenticar(self, agente=False):
        token = self.server.coordenador.config["token_agente" if agente else "token_admin"]
        recebido = self.headers.get("Authorization", "")
        if not hmac.compare_digest(recebido.encode(), ("Bearer " + token).encode()):
            raise ErroAPI(401, "Chave de acesso inválida.")
        origem = self.headers.get("Origin")
        if origem and urlsplit(origem).netloc.lower() != self.headers.get("Host", "").lower():
            raise ErroAPI(403, "Origem da solicitação não permitida.")

    def ler(self):
        if self.headers.get("Content-Type", "").split(";")[0] != "application/json":
            raise ErroAPI(415, "Envie application/json.")
        try:
            tamanho = int(self.headers.get("Content-Length", "0"))
            if not 0 < tamanho <= 100_000:
                raise ErroAPI(413, "Corpo da solicitação excede 100 KB ou está vazio.")
            dados = json.loads(self.rfile.read(tamanho), parse_constant=lambda _: (_ for _ in ()).throw(ValueError()))
            if not isinstance(dados, dict):
                raise ValueError()
            return dados
        except (ValueError, UnicodeError):
            raise ErroAPI(400, "JSON inválido.") from None

    def do_GET(self):
        try:
            rota = urlsplit(self.path).path
            if rota == "/api/estado":
                self.autenticar()
                estado = self.server.coordenador.estado()
                estado["enderecos_painel"] = [f"http://{ip}:{self.server.server_port}" for ip in ips_locais()]
                return self.enviar(200, estado)
            if rota.startswith("/api/configuracao/"):
                self.autenticar()
                identidade = rota.rsplit("/", 1)[1]
                coord = self.server.coordenador
                with coord.lock:
                    if identidade not in coord.nos:
                        raise ErroAPI(404, "PC não encontrado.")
                    dados = {"id": identidade, "coordenador": "http://IP_DO_COORDENADOR:8080",
                             "token": coord.config["token_agente"]}
                return self.enviar(200, dados, arquivo=identidade + ".json")
            arquivos = {"/": "index.html", "/index.html": "index.html", "/app.js": "app.js", "/style.css": "style.css"}
            if rota not in arquivos:
                raise ErroAPI(404, "Página não encontrada.")
            caminho = WEB / arquivos[rota]
            tipo = mimetypes.guess_type(caminho.name)[0] or "application/octet-stream"
            self.enviar(200, caminho.read_bytes(), tipo + "; charset=utf-8")
        except (ErroAPI, ValueError) as erro:
            self.enviar(getattr(erro, "codigo", 400), {"erro": str(erro)})
        except (BrokenPipeError, ConnectionResetError):
            pass

    def do_POST(self):
        try:
            rota = urlsplit(self.path).path
            self.autenticar(agente=rota.startswith("/api/agente/"))
            dados = self.ler()
            coord = self.server.coordenador
            if rota == "/api/agente/sincronizar":
                resposta = coord.sincronizar(dados)
            elif rota == "/api/agente/solucao":
                resposta = coord.solucao(dados)
            elif rota == "/api/config":
                coord.configurar(dados)
                resposta = {"ok": True}
            elif rota == "/api/controle":
                coord.controlar(dados.get("acao"))
                resposta = {"ok": True}
            elif rota.startswith("/api/nos/"):
                coord.configurar_no(rota.rsplit("/", 1)[1], dados)
                resposta = {"ok": True}
            else:
                raise ErroAPI(404, "Rota não encontrada.")
            self.enviar(200, resposta)
        except (ErroAPI, ValueError) as erro:
            self.enviar(getattr(erro, "codigo", 400), {"erro": str(erro)})
        except (BrokenPipeError, ConnectionResetError):
            pass

    def setup(self):
        super().setup()
        self.connection.settimeout(12)


def ips_locais():
    try:
        return sorted({endereco[4][0] for endereco in socket.getaddrinfo(socket.gethostname(), None, socket.AF_INET)
                       if not endereco[4][0].startswith("127.")})
    except OSError:
        return []


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--host", default="0.0.0.0")
    parser.add_argument("--porta", type=int, default=8080)
    parser.add_argument("--fonte", default="http://172.16.3.30:8001", help="Fonte inicial; depois use o painel.")
    parser.add_argument("--nome", default="Cluster", help="Nome inicial do grupo na fonte.")
    parser.add_argument("--config", default=".cluster/config.json", help="Arquivo privado de configuração persistente.")
    args = parser.parse_args()
    if not 1 <= args.porta <= 65535:
        parser.error("Use uma porta entre 1 e 65535.")
    coord = None
    servidor = None
    try:
        coord = Coordenador(args.config, url_http(args.fonte), args.nome)
        servidor = Servidor((args.host, args.porta), coord)
        print(f"Painel local: http://127.0.0.1:{args.porta}", flush=True)
        if args.host == "0.0.0.0":
            for ip in ips_locais():
                print(f"Painel na rede: http://{ip}:{args.porta}", flush=True)
        elif args.host not in ("127.0.0.1", "localhost"):
            print(f"Painel no endereço configurado: http://{args.host}:{args.porta}", flush=True)
        print(f"Chave de acesso do painel: {coord.config['token_admin']}", flush=True)
        print("Cluster pausado. Abra o painel para configurar e iniciar. Ctrl+C encerra.", flush=True)
        coord.iniciar()
        servidor.serve_forever(poll_interval=0.3)
    except KeyboardInterrupt:
        print("\nCoordenador encerrado.")
    except (OSError, ValueError) as erro:
        print(f"Falha ao iniciar o coordenador: {erro}")
        return 1
    finally:
        if coord:
            coord.fechar()
        if servidor:
            servidor.server_close()
    return 0
