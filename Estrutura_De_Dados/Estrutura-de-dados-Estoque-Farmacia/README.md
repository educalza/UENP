# Gerenciamento de Estoque de Farmácia

## Descrição do Projeto

Este projeto tem como objetivo praticar o uso de tipos abstratos de dados e estruturas do tipo Lista. Você é o responsável pelo estoque de medicamentos de uma grande farmácia. A partir de um arquivo de entrada (`entrada.txt`), seu sistema deve ser capaz de realizar o processamento do estoque da farmácia que você está gerenciando.

Nesse trabalho, você deverá implementar a estrutura `medicamento`, que irá conter o nome (char[20]), código de identificação (int), valor (float) e data de validade (int[3]). Para armazenar esses medicamentos, é necessário ter a estrutura `lista`, que será alocada de forma dinâmica.

O programa testador deverá ser capaz de ler as instruções do arquivo texto de entrada e realizar as devidas operações no TAD estoque. O programa (`trabalho1.c`) lerá os dados de entrada a partir de um arquivo de entrada (`entrada.txt`). O arquivo de entrada é basicamente uma lista de comandos (um por linha) em formato texto. O último comando é a palavra FIM, que indica o final do arquivo.

## Formato do Arquivo de Entrada

O formato do arquivo de entrada (`entrada.txt`) é exemplificado abaixo:

```plaintext
INSERIR 1234 "Paracetamol" 12.99 15 7 2023
REMOVER 1234
INSERIR 5678 "Ibuprofeno" 23.50 1 8 2024
CONSULTAR 5678
FIM
```


## Estrutura de Dados

### Medicamento

A estrutura `medicamento` contém:
- `char nome[20]`: Nome do medicamento
- `int codigo`: Código de identificação
- `float valor`: Valor do medicamento
- `int validade[3]`: Data de validade (dia, mês, ano)

### Lista

A estrutura `lista` é alocada dinamicamente e contém os medicamentos.

## Funcionalidades

O sistema é capaz de realizar as seguintes operações:
- `INSERIR`: Insere um novo medicamento no estoque.
- `REMOVER`: Remove um medicamento do estoque com base no código de identificação.
- `CONSULTAR`: Consulta informações de um medicamento específico no estoque com base no código de identificação.
- `FIM`: Indica o fim do arquivo de entrada e termina a execução do programa.

## Compilação e Execução

Para compilar o programa, use o comando:

```bash
gcc trabalho1.c estoque.c -o trabalho
.\trabalho.exe

