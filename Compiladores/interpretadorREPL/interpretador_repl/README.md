## 🧠 Interprete de Expressões Matemáticas em Dart (REPL)

Este projeto é um **interpretador simples no estilo REPL (Read-Eval-Print Loop)**, escrito em **Dart**, baseado no capítulo 2 do livro *"Compiladores: Princípios, Técnicas e Ferramentas (2ª edição)"* de Aho, Lam, Sethi e Ullman - Dragon Book.

Ele é capaz de:

* Avaliar expressões matemáticas com `+`, `-`, `*`, `/`
* Declarar variáveis com base em expressões
* Reutilizar variáveis já declaradas
* Exibir os resultados das expressões

---

### 📌 Exemplo de uso

```bash
>>> 3 + 4
Resultado: 7

>>> x = 10
Resultado: 10

>>> y = x * 2 + (3 - 1)
Resultado: 22

>>> y
Resultado: 22

>>> z
Erro: Variável 'z' não definida.
```

---

### 📁 Estrutura do Projeto

```
bin/
├── lexer.dart        # Analisa o texto de entrada e gera tokens
├── parser.dart       # Avalia os tokens como expressões matemáticas
├── token.dart        # Define os tipos de tokens e a classe Token
└── main.dart         # Interface REPL
```

---

### 🚀 Como executar

1. **Instale o SDK do Dart**:
   [https://dart.dev/get-dart](https://dart.dev/get-dart)

2. **Clone o repositório**:

```bash
git clone https://github.com/seu-usuario/nome-do-repo.git
cd nome-do-repo
```

3. **Execute o programa**:

```bash
dart run bin/main.dart
```

---

### 🛠 Tecnologias usadas

* [Dart](https://dart.dev/) — Linguagem moderna, simples e segura
* Conceitos de compiladores: análise léxica, parsing e ambiente de execução

---

### 📚 Base teórica

* Livro: *Compiladores: Princípios, Técnicas e Ferramentas*, 2ª edição — Aho, Lam, Sethi, Ullman
* Capítulo 2: Introdução à construção de interpretadores

*Obs: a atividade proposta foi feita com auxílio de Inteligência Artificial*
