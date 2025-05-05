## Algoritmo de simplificação de gramáticas livres de contexto

Trabalho de Teoria da Computação - Simplificador de Gramática Livre de Contexto

## Funcionalidades

- Leitura de gramáticas a partir de um arquivo JSON.

- Funções do algoritmo:
  - **Simplificação**
    - a) símbolos inúteis/inalcançáveis ✓
    - b) produções vazias ✓
    - c) substituição de produções ✓

  - **Formas Normais**
    - ~~a) Chomsky~~ X
    - ~~b) Greibach~~ X
  - **Melhorias**
    - ~~a) Fatoração à esquerda~~ X
    - ~~b) Remoção de recursão à esquerda~~ X

- Criação de um arquivo output em JSON contendo as novas gramáticas simplificadas.

## Execução do programa & Exemplos

É necessário o uso de um arquivo para que o programa funcione

### Arquivo de entrada JSON gramáticas livres de contexto

```json
{
    "S": ["aAa", "bBb"],
    "A": ["a", "aA"]
}
```

### Arquivo de saída JSON gramáticas livres de contexto

```json
{
    "S": ["aAa"],
    "A": ["a","aA"]
}
```

Percebe-se que o "bBb" foi removido, por ser inútil

