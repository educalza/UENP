import 'token.dart';
import 'lexer.dart';

// O Parser recebe uma lista de tokens e transforma em estruturas que podem ser avaliadas
class Parser {
  final List<Token> tokens;
  int current = 0; // Posição atual na lista de tokens

  Parser(this.tokens);

  // Retorna o token atual ou um token EOF se chegou ao fim
  Token get _currentToken =>
      current < tokens.length ? tokens[current] : Token(TokenType.eof, '');

  // Verifica se o token atual é do tipo esperado, avança se for
  bool _match(TokenType type) {
    if (_currentToken.type == type) {
      current++;
      return true;
    }
    return false;
  }

  // Garante que o token atual é do tipo esperado, senão lança erro
  Token _consume(TokenType type, String message) {
    if (_currentToken.type == type) {
      return tokens[current++];
    }
    throw Exception("Erro de sintaxe: $message");
  }

  /// Analisa uma instrução:
  /// Pode ser uma atribuição (ex: x = 3 + 4) ou uma expressão simples (ex: 3 + 4)
  dynamic parseStatement(Map<String, num> env) {
    // Verifica se é uma atribuição: identifier '=' expression
    if (_currentToken.type == TokenType.identifier &&
        tokens.length > current + 1 &&
        tokens[current + 1].type == TokenType.equal) {
      final name = _consume(TokenType.identifier, "Esperado nome de variável");
      _consume(TokenType.equal, "Esperado '='");
      final value = _parseExpression(env);
      env[name.value] = value; // Salva a variável no ambiente
      return value;
    }

    // Caso contrário, é apenas uma expressão matemática
    return _parseExpression(env);
  }

  /// expression → term (('+' | '-') term)* ;
  /// Trata soma e subtração
  num _parseExpression(Map<String, num> env) {
    var value = _parseTerm(env); // Primeiro termo

    // Enquanto houver + ou -, continue avaliando
    while (_match(TokenType.plus) || _match(TokenType.minus)) {
      final operator = tokens[current - 1];
      final right = _parseTerm(env); // Próximo termo

      if (operator.type == TokenType.plus) {
        value += right;
      } else {
        value -= right;
      }
    }

    return value;
  }

  /// term → factor (('*' | '/') factor)* ;
  /// Trata multiplicação e divisão
  num _parseTerm(Map<String, num> env) {
    var value = _parseFactor(env); // Primeiro fator

    // Enquanto houver * ou /, continue avaliando
    while (_match(TokenType.star) || _match(TokenType.slash)) {
      final operator = tokens[current - 1];
      final right = _parseFactor(env);

      if (operator.type == TokenType.star) {
        value *= right;
      } else {
        value /= right;
      }
    }

    return value;
  }

  /// factor → NUMBER | IDENTIFIER | '(' expression ')' ;
  /// Trata números, variáveis ou expressões entre parênteses
  num _parseFactor(Map<String, num> env) {
    final token = _currentToken;

    if (_match(TokenType.number)) {
      return num.parse(token.value); // Converte o número string para num
    }

    if (_match(TokenType.identifier)) {
      // Verifica se a variável foi declarada
      if (!env.containsKey(token.value)) {
        throw Exception("Variável '${token.value}' não definida.");
      }
      return env[token.value]!;
    }

    if (_match(TokenType.lparen)) {
      final value = _parseExpression(env); // Avalia expressão dentro dos parênteses
      _consume(TokenType.rparen, "Esperado ')'");
      return value;
    }

    throw Exception("Fator inesperado: ${token.value}");
  }
}
