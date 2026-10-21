# Minerador otimizado para CPU

Projeto independente para mineração na rede local usando apenas CPU. Mantém
o mesmo protocolo de hash e as otimizações em C da versão principal.

## Executar

Extraia o ZIP, abra um terminal na pasta `minerador_cpu` e execute:

```powershell
python minerador.py --servidor http://172.16.3.30:8001 --nome Eduardo
```

Use um nome diferente para cada máquina. O programa acompanha as rodadas e
começa a calcular quando o servidor disponibiliza um trabalho. Sem trabalho,
os processos aguardam. Para encerrar todos os processos, pressione **Ctrl+C**.

É necessário Python 3.10 ou mais recente. A biblioteca C incluída foi compilada
para Windows de 64 bits e testada com Python 3.13 de 64 bits. Não é necessário
instalar pacotes Python adicionais. Se a biblioteca C não puder ser carregada,
o modo automático usa a implementação Python.

## Desempenho

Por padrão, usa o motor C disponível, sem pausas, e a melhor quantidade de
processos registrada por um benchmark válido para o ambiente atual. Sem essa
medição, utiliza todas as CPUs lógicas disponíveis.

O cálculo em C reaproveita o estado SHA-256 das partes fixas, atualiza apenas
os dígitos necessários do nonce e verifica a dificuldade diretamente nos bytes.
Os processos percorrem sequências diferentes de nonces. A comunicação HTTP
continua enquanto os outros processos calculam.

Para medir este computador, sem acessar o servidor:

```powershell
python minerador.py --benchmark --duracao 3
```

O resultado fica em `desempenho.json`. Faça a medição com outros programas
pesados fechados; a velocidade real também depende dos dados de cada rodada.

Para limitar a quantidade de processos:

```powershell
python minerador.py --servidor http://172.16.3.30:8001 --nome Eduardo --processos 4
```

As opções de motor são `auto`, `nativo`, `nativo-evp` e `python`. Para exigir o
motor C, acrescente `--motor nativo`. A pausa padrão é zero. `--pausa 0.01`
adiciona 10 milissegundos de descanso entre lotes de cada processo. A consulta
de novas rodadas ocorre a cada 0,25 segundo e pode ser ajustada por `--consulta`.

## Recompilar o motor C

A biblioteca Windows já está incluída. Para gerar uma biblioteca para outro
sistema ou recompilar após alterar o código, instale GCC ou Clang e execute:

```powershell
python compilar.py
```

A via SHA direta utiliza os headers oficiais do OpenSSL. Sua pasta pode ser
indicada em `OPENSSL_INCLUDE_DIR`. Na ausência desses headers, a compilação
tenta automaticamente a via EVP, que também processa os lotes em C. Para
compilar somente essa via, use `python compilar.py --somente-evp`.

O motor carrega o OpenSSL disponível no Python ou no sistema. A via direta só
é ativada com a mesma versão principal usada na compilação; as funções SHA
diretas estão obsoletas no OpenSSL 3 e o modo EVP permanece como alternativa.
O resultado é conferido contra o protocolo original antes de cada trabalho.

## Testes

```powershell
python -m unittest -v
```

Os testes verificam os hashes, limites dos nonces, divisão do trabalho,
troca de rodada, encerramento e comunicação com um servidor HTTP simulado
em `127.0.0.1`. O servidor da rede local não é acessado pelos testes.
