import 'token.dart'; // Assumindo que sua enum e classe Token estão aqui

class Lexer {
  final String input;
  int currentPosition = 0;

  Lexer(this.input);

  List<Token> tokenize() {
    final tokens = <Token>[];

    while (currentPosition < input.length) {
      final currentChar = input[currentPosition];

      if (_isWhitespace(currentChar)) {
        currentPosition++;
        continue;
      }

      final token = _nextToken();
      if (token != null) {
        tokens.add(token);
      } else {
        throw Exception('Caractere desconhecido: $currentChar');
      }
    }

    tokens.add(Token(TokenType.eof, 'EOF'));
    return tokens;
  }

  Token? _nextToken() {
    final remaining = input.substring(currentPosition);

    final patterns = <RegExp, TokenType>{ // RegExp - Classe de expressões regulares de Dart.
      RegExp(r'^\d+(\.\d+)?'): TokenType.number,
      RegExp(r'^[a-zA-Z_][a-zA-Z0-9_]*'): TokenType.identifier,
      RegExp(r'^\+'): TokenType.plus,
      RegExp(r'^\-'): TokenType.minus,
      RegExp(r'^\*'): TokenType.star,
      RegExp(r'^\/'): TokenType.slash,
      RegExp(r'^\='): TokenType.equal,
      RegExp(r'^\('): TokenType.lparen,
      RegExp(r'^\)'): TokenType.rparen,
    };

    for (final entry in patterns.entries) {
      final match = entry.key.firstMatch(remaining);
      if (match != null) {
        final value = match.group(0)!;
        currentPosition += value.length;
        return Token(entry.value, value);
      }
    }

    return null;
  }

  bool _isWhitespace(String char) {
    return RegExp(r'\s').hasMatch(char);
  }
}
