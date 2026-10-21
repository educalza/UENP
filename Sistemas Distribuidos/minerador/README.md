# Minerador de rede local

## Cluster de seis computadores

Para coordenar **2 PCs CPU e 4 PCs RTX**, com Windows/Linux e painel web local:

```powershell
python coordenador.py
```

Abra `http://127.0.0.1:8080` e use a chave exibida no terminal. O painel permite
baixar a configuração de cada agente, iniciar/pausar o cluster e ajustar cada PC.
A fonte padrão continua sendo `http://172.16.3.30:8001`.
Siga o passo a passo completo em [CLUSTER.md](CLUSTER.md).
O pacote `cluster_minerador.zip` reúne os arquivos necessários para copiar aos PCs.

## Minerador individual

O minerador usa processos paralelos, sem pausas. O coordenador consulta o
servidor enquanto os outros processos calculam hashes. Cada processo percorre
uma sequência distinta de nonces, inclusive ao completar o contador de 64 bits.

O motor nativo executa os lotes em C, reutiliza o estado SHA-256 das partes fixas
e verifica a dificuldade diretamente nos bytes. O nonce decimal é atualizado
por soma, sem reconverter todos os dígitos em cada tentativa. O formato de hash é o mesmo
de `protocolo.py`, conferido antes de começar cada trabalho. Se o motor nativo
não estiver disponível, o programa usa automaticamente o motor Python.

Execute na pasta do projeto, com Python 3 instalado:

```powershell
python minerador.py --servidor http://172.16.3.30:8001 --nome Eduardo
```

Para limitar a quantidade de processos:

```powershell
python minerador.py --servidor http://172.16.3.30:8001 --nome Eduardo --processos 4
```

## NVIDIA RTX / CUDA

Para minerar na GPU NVIDIA:

```powershell
python minerador.py --servidor http://172.16.3.30:8001 --nome Eduardo --motor cuda
```

O motor CUDA usa um único processo alimentando milhares de threads na GPU.
Não é necessário informar `--processos`; valores maiores que 1 são rejeitados
para evitar disputar a mesma GPU entre processos. `--pausa 0` continua sendo
o padrão. `--motor auto` mantém a seleção anterior de CPU C/Python; CUDA é
ativado explicitamente por `--motor cuda`.

O driver NVIDIA deve estar instalado. Para preparar o compilador CUDA local:

```powershell
python preparar_cuda.py
```

Esse comando baixa o pacote oficial `nvidia-cuda-nvrtc-cu12==12.9.86` para
`.cuda-deps/`, sem instalar o CUDA Toolkit nem alterar pacotes globais do Python.
Essa preparação já foi realizada nesta máquina para a RTX 4050 Laptop.

O código CUDA C em `native/minerador_cuda.cu` é compilado por NVRTC para a
arquitetura da GPU. Os programas compilados ficam em `native/cuda_cache/` e
são reutilizados. O primeiro trabalho com um formato ainda não compilado pode
demorar mais para começar. Partes fixas e modelos de mensagem ficam na GPU;
apenas os parâmetros do lote e um pequeno resultado são transferidos.

Cada lote é limitado para permitir acompanhar mudanças de rodada. O hash da
primeira thread de cada lote é conferido com o protocolo original na CPU;
qualquer solução encontrada também é conferida antes de ser enviada. Como
os cálculos são paralelos, o contador inclui todo o lote realmente calculado,
inclusive tentativas simultâneas após o nonce que encontrou a primeira prova.

Para medir a RTX, sem acessar o servidor:

```powershell
python minerador.py --benchmark --motor cuda --duracao 3
```

O resultado da GPU fica em `desempenho_cuda.json`, separado do benchmark de CPU.
A comparação deve usar o mesmo trabalho e condições semelhantes de carga,
energia e temperatura. Não é usado um algoritmo de mineração de Bitcoin: a GPU
calcula exatamente o JSON e o SHA-256 duplo específicos deste projeto.

No motor de CPU, sem `--processos`, o programa usa a melhor quantidade medida pelo benchmark
salvo nesta máquina. Sem benchmark válido, usa todas as CPUs lógicas disponíveis.
Uma alteração do código, do motor ou da versão do Python invalida o resultado
salvo. O benchmark usa um trabalho sintético; o desempenho real varia conforme
os dados do servidor, a temperatura e outros programas em execução.

## Compilação e medição local

Para compilar o motor nativo, é necessário GCC ou Clang disponível no terminal:

```powershell
python compilar.py
```

O compilador gera a biblioteca v2 em `native/`; não instala pacotes. O motor
carrega o OpenSSL disponível no Python ou no sistema. Encerre o minerador antes
de recompilar a biblioteca.

Para a via SHA direta, os headers de desenvolvimento do OpenSSL precisam estar
no caminho do compilador. É possível indicar sua pasta em `OPENSSL_INCLUDE_DIR`.
A compilação tenta automaticamente a via EVP caso os headers não estejam
disponíveis. Para compilar somente a via EVP, use `python compilar.py --somente-evp`.

A via direta é opcional: usa `SHA256_CTX` dos headers oficiais e só é ativada
se a versão principal do OpenSSL carregado corresponder à usada na compilação.
As funções `SHA256_Init/Update/Final` estão obsoletas no OpenSSL 3; quando não
estiverem disponíveis, o programa mantém a via EVP. Ambas passam pela mesma
verificação de equivalência antes de minerar.

Para comparar as quantidades de processos e salvar a melhor configuração:

```powershell
python minerador.py --benchmark --duracao 2
```

Esse teste não acessa a rede. Ele inclui aquecimento e mede apenas o período de
cálculo. O resultado fica em `desempenho.json`. Faça o teste com os outros
programas pesados fechados. Para comparar com a implementação Python:

```powershell
python minerador.py --benchmark --motor python --duracao 2
```

O último benchmark salvo é aplicado apenas ao motor e ambiente correspondentes.
Para forçar um motor, use `--motor nativo`, `--motor nativo-evp` ou `--motor python`
no comando de mineração. `nativo-evp` permite comparar a via EVP com a via direta
na mesma versão do C. O padrão é `--motor auto`.

## Ajustes e encerramento

`--pausa 0.01` adiciona uma pausa de 10 milissegundos entre lotes de cada
processo. O padrão é `--pausa 0`. A taxa exibida em `hashes/s` soma o trabalho
dos processos; a quantidade ideal de processos pode variar conforme o computador.

O intervalo de consulta de novas rodadas é de 0,25 segundo. É possível ajustar
com `--consulta`, entre 0,1 e 60 segundos. Intervalos menores geram mais consultas
ao servidor; o trabalho de cálculo continua independente dessas requisições.

O servidor precisa estar ativo, acessível pela rede e usar o mesmo formato de
hash de `protocolo.py`. Sem trabalho disponível, os processos aguardam.
Use **Ctrl+C** para encerrar o coordenador e todos os processos de cálculo.

Os testes usam apenas um servidor simulado em `127.0.0.1`:

```powershell
python -m unittest -v
```

Os testes também verificam equivalência dos motores, dificuldades pares e
ímpares, mudanças na quantidade de dígitos do nonce, overflow de 64 bits,
descarte de rodadas antigas, encerramento e seleção da configuração medida.
Os testes CUDA executam na GPU real quando driver e NVRTC estão disponíveis;
nos outros ambientes são ignorados. Não é preciso acessar o servidor da LAN.
