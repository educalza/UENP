# Cluster de mineração: 2 PCs CPU + 4 PCs NVIDIA RTX

O painel coordena os seis computadores Windows/Linux e consulta a fonte
**http://172.16.3.30:8001**. Somente o coordenador envia presença, busca trabalhos
e entrega provas à fonte. Todos usam o mesmo nome na fonte; no painel, cada PC
tem sua própria identificação.

```text
Fonte 172.16.3.30:8001
           ↕
Coordenador + painel :8080
     ↕     ↕     ↕     ↕     ↕     ↕
   PC01  PC02  PC03  PC04  PC05  PC06
   CPU   CPU   RTX   RTX   RTX   RTX
```

## 1. Inicie o coordenador

Extraia `cluster_minerador.zip` ou use a pasta atual do projeto.
Requer Python **3.10 ou superior**, de 64 bits. No Linux, use `python3` onde os
exemplos mostram `python`.

```powershell
python coordenador.py
```

Abra **http://127.0.0.1:8080** neste computador. Copie a **chave de acesso do
painel** exibida no terminal para a tela de entrada. A chave permanece a mesma
nas próximas execuções, pois fica em `.cluster/config.json`.

O coordenador inicia **pausado**. Configure seu nome na fonte no painel.
A configuração padrão já aponta para `http://172.16.3.30:8001`.

O terminal também mostra os endereços de rede disponíveis. Escolha o IP da
mesma rede da fonte e dos seis computadores. Nesta máquina foi detectado
`172.16.3.107` durante a preparação; confirme se continua sendo o IP atual.
Não confunda o endereço do coordenador com o endereço da fonte.

Porta ou endereço inicial diferente:

```powershell
python coordenador.py --porta 8080 --fonte http://172.16.3.30:8001 --nome MinhaEquipe
```

`--fonte` e `--nome` definem os valores da **primeira execução**. Se já existir
um arquivo de configuração, altere esses campos pelo painel. Para outro
cluster independente, use `--config caminho/outro-cluster.json`.

## 2. Prepare os computadores

Copie/extraia o **mesmo pacote completo** em todos os seis PCs. O coordenador
também pode executar um agente, se ele for um desses PCs; nesse caso são dois
programas abertos nessa máquina. O programa coordenador sozinho não minera.

| ID | Perfil inicial | Preparação |
| --- | --- | --- |
| pc-01 | CPU, todos os núcleos | Motor C |
| pc-02 | CPU, todos os núcleos | Motor C |
| pc-03 | NVIDIA RTX | Driver NVIDIA + NVRTC |
| pc-04 | NVIDIA RTX | Driver NVIDIA + NVRTC |
| pc-05 | NVIDIA RTX | Driver NVIDIA + NVRTC |
| pc-06 | NVIDIA RTX | Driver NVIDIA + NVRTC |

**CPU no Windows:** o pacote inclui `native/minerador_native_v2.dll` para
Windows x64. Use Python x64. Para recompilar, instale GCC/Clang e execute
`python compilar.py`. O perfil `auto` usa C quando disponível e Python como
alternativa; o painel mostra o motor efetivamente usado.

**CPU no Linux:** compile a biblioteca no próprio computador:

```sh
# Em Debian/Ubuntu, se as dependências ainda não estiverem instaladas:
sudo apt install python3 python3-pip build-essential libssl-dev
python3 compilar.py
```

**RTX, Windows ou Linux:** instale o driver NVIDIA apropriado e prepare o
compilador local:

```powershell
python preparar_cuda.py
```

Esse comando baixa o pacote oficial NVRTC CUDA 12.9 para `.cuda-deps`, que não
acompanha o ZIP. Requer acesso à internet nessa preparação ou um compilador
NVRTC compatível já instalado. A versão escolhida exige driver compatível
com CUDA 12.9. É possível apontar `MINERADOR_NVRTC` para uma biblioteca NVRTC
compatível; no Windows, mantenha a biblioteca `nvrtc-builtins` ao lado dela.
No Linux, o NVRTC já instalado pode ser encontrado pelo carregador de bibliotecas.

CUDA usa a primeira GPU NVIDIA visível ao driver e um processo por agente.
Cada agente usa **CPU ou GPU**, conforme o perfil. Se CUDA falhar, o erro aparece
no painel; o agente tenta novamente e permite trocar o motor nos ajustes.
O primeiro trabalho pode demorar mais por causa da compilação do kernel.

## 3. Conecte os agentes

No painel, clique em **Conectar computadores**:

1. Confira o endereço do coordenador na LAN, por exemplo
   `http://172.16.3.107:8080` **se esse ainda for o IP desse computador**.
2. Baixe uma configuração para cada PC: `pc-01.json` até `pc-06.json`.
3. Coloque o arquivo correspondente na pasta do projeto daquele PC.
4. Na pasta do projeto, execute:

```powershell
python agente_cluster.py --config pc-01.json
```

No segundo PC, use `pc-02.json`, e assim por diante. **Não use o mesmo ID em
dois computadores.** Um segundo agente com o mesmo ID é recusado enquanto o
anterior estiver conectado. Depois de um encerramento/reinício, a sessão
anterior expira em até 10 segundos.

Se o IP do coordenador mudar, baixe os arquivos novamente ou substitua o
endereço ao iniciar o agente:

```powershell
python agente_cluster.py --config pc-01.json --coordenador http://NOVO_IP:8080
```

Libere a porta **TCP 8080 de entrada no coordenador** para a rede local.
Os agentes fazem conexões de saída e não precisam de portas de entrada.
O coordenador também precisa alcançar a fonte na porta **8001**.
O pacote não altera o firewall automaticamente.

As chaves do painel e dos agentes são diferentes. Mantenha privados o arquivo
`.cluster/config.json` e os arquivos `pc-XX.json`. Use o serviço na rede local
confiável; o HTTP padrão não cifra as chaves. O painel não executa comandos
arbitrários nos computadores: controla apenas a mineração dos agentes iniciados.

## 4. Inicie a mineração

Clique em **Iniciar cluster**. Quando houver trabalho na fonte e agentes
conectados, eles começam automaticamente. O painel mostra:

- Conectividade e velocidade individual e total em hashes/s.
- Trabalho atual, dificuldade e nome usado na fonte.
- Blocos aceitos, provas recusadas e eventos da sessão.
- Pausa/ativação por PC, troca entre CPU/CUDA e quantidade de processos CPU.

Nos ajustes de CPU, **0 usa todos os núcleos disponíveis**. O número solicitado
é limitado à quantidade de CPUs disponíveis no PC. CUDA sempre usa um processo.
A pausa entre lotes continua sendo zero; não é necessário informar `--pausa`.

Cada computador recebe uma das seis sequências de nonces. Os processos locais
dividem essa sequência entre si, mesmo quando os PCs têm quantidades distintas
de núcleos. Não há colisão entre as partições, inclusive ao dar a volta no
contador de 64 bits. Uma mudança de perfil pode repetir parte da busca do
próprio PC, mas não invade a sequência dos demais.

As soluções são verificadas no coordenador antes de chegar à fonte. Provas de
rodadas antigas são descartadas e apenas um envio por rodada fica em andamento.
O painel conta um bloco como aceito somente após HTTP 201 da fonte.

Se a fonte ficar indisponível, o coordenador retira o trabalho e tenta reconectar;
os agentes param ao receber essa atualização. Se um agente perder o coordenador,
ele para ao detectar a falha e tenta reconectar. Os tempos incluem a detecção da
falha e o limite HTTP, normalmente alguns segundos, além da conclusão do lote
de cálculo atual. Não execute outro minerador independente no mesmo PC durante
o uso do agente, pois ambos disputariam CPU/GPU.

**Fonte sem blocos** deve responder HTTP 200 com `{"trabalho": null}`.
Conexão recusada ou timeout indicam problema de disponibilidade, endereço,
porta ou rede, mesmo quando não há blocos. O painel diferencia essas situações.

As velocidades são estimativas baseadas nos relatórios dos agentes. Contadores
de tentativas podem não incluir o último lote interrompido. Estatísticas e
atividade pertencem à sessão atual do coordenador; configurações e chaves
persistem. Pause o cluster e use Ctrl+C para encerrar agentes e coordenador.

## Validação local

```powershell
python -m unittest test_minerador test_motores test_cuda test_cluster -v
```

Os testes usam uma fonte simulada local, validam os seis slots, sessões,
autenticação, pausas, mudanças de rodada, nonces e prova enviada por agente CPU
e CUDA real quando disponível. Testes CUDA são ignorados se GPU/driver/NVRTC
não estiverem disponíveis. A interface foi conferida no navegador no Windows.
As outras máquinas e o Linux precisam ser verificados após a instalação.

O minerador individual e a cópia `minerador_cpu/` continuam disponíveis.
