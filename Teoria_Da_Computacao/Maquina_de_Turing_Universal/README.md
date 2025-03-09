# Máquina de Turing Universal

Este projeto simula a execução de uma Máquina de Turing Universal (MTU) usando Python. Ele processa uma fita de entrada e determina se o estado final é de aceitação ou rejeição para a entrada fornecida.

## Estrutura do Projeto

O projeto requer dois arquivos principais de entrada:

- **Configuração da Máquina (JSON):** Arquivo JSON que define a configuração e as transições da Máquina de Turing.
- **Fita de Entrada (TXT):** Arquivo TXT que contém as sequências de entrada a serem processadas pela MTU.

O programa gera um arquivo `saida.txt`, onde cada linha representa a fita final após o processamento de cada entrada.

## Estrutura do Arquivo JSON de Configuração

O arquivo JSON de configuração da máquina deve conter os seguintes elementos:

- **`transitions`**: Lista de transições, onde cada transição é um dicionário com os elementos:
  - `from`: Estado de origem.
  - `read`: Símbolo esperado na posição do cabeçote.
  - `write`: Símbolo que será escrito na posição do cabeçote.
  - `to`: Novo estado após a transição.
  - `dir`: Direção do movimento do cabeçote (`R` para direita, `L` para esquerda).

- **`white`**: Símbolo que representa o espaço em branco na fita.
- **`initial`**: Estado inicial da máquina.
- **`final`**: Lista de estados finais de aceitação.

### Exemplo de Arquivo JSON

```json
{
    "initial" : 0,
    "final" : [4],
    "white" : "_",
    "transitions" : [
        {"from": 0, "to": 1, "read": "a", "write": "A", "dir":"R"},
        {"from": 1, "to": 1, "read": "a", "write": "a", "dir":"R"},
        {"from": 1, "to": 1, "read": "B", "write": "B", "dir":"R"},
        {"from": 1, "to": 2, "read": "b", "write": "B", "dir":"L"},
        {"from": 2, "to": 2, "read": "B", "write": "B", "dir":"L"},
        {"from": 2, "to": 2, "read": "a", "write": "a", "dir":"L"},
        {"from": 2, "to": 0, "read": "A", "write": "A", "dir":"R"},
        {"from": 0, "to": 3, "read": "B", "write": "B", "dir":"R"},
        {"from": 3, "to": 3, "read": "B", "write": "B", "dir":"R"},
        {"from": 3, "to": 4, "read": "_", "write": "_", "dir":"L"}      
    ]
}
```
Neste exemplo, a máquina começa no estado `q0`, lê `1`, escreve `0`, move-se para o estado `q1` e à direita. Em `q1`, lê `0`, escreve `1`, move-se à esquerda e muda para o estado final `qf`.

## Estrutura do Código

O código principal (`main.py`) realiza as seguintes operações:

1. **Carregamento da Configuração JSON**: Abre e lê o arquivo JSON de configuração da máquina.
2. **Carregamento da Fita de Entrada**: Abre e lê o arquivo TXT de entrada, onde cada linha representa uma fita.
3. **Simulação da Máquina de Turing**: Para cada linha do arquivo de entrada:
   - Processa a fita de acordo com as regras da máquina.
   - Expande a fita, caso o cabeçote ultrapasse os limites.
4. **Verificação de Aceitação**: Imprime `1` se o estado final for de aceitação, ou `0` caso contrário.

## Como Usar

1. Crie o arquivo JSON de configuração da máquina com a estrutura descrita e ajuste o caminho do arquivo no código (`ler_arquivojson`).
2. Crie o arquivo TXT de entrada, onde cada linha é uma fita a ser processada, e ajuste o caminho do arquivo no código (`ler_arquivotxt`).
3. Execute o script:

   ```bash
   python main.py
