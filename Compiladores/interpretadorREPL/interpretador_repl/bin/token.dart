enum TokenType {
  number,      // ex: 123
  identifier,  // ex: x, y, total
  plus,        // +
  minus,       // -
  star,        // *
  slash,       // /
  equal,       // =
  lparen,      // (
  rparen,      // )
  eof          // fim da entrada
}

class Token {
  final TokenType type;
  final String value;

  Token(this.type, this.value);

  @override
  String toString() {
    return "Token{type=$type, value='$value'}";
  }
}
