import 'dart:io';
import 'lexer.dart';
import 'parser.dart';
import 'token.dart';

void main() {
  print('Interpretador REPL - Digite "sair" para encerrar.');
  final env = <String, num>{};

  while (true) {
    stdout.write('>>> ');
    final input = stdin.readLineSync();

    if (input == null || input.trim().toLowerCase() == 'sair') break;

    try {
      final lexer = Lexer(input);
      final tokens = lexer.tokenize();
      final parser = Parser(tokens);
      final result = parser.parseStatement(env);
      print('Resultado: $result');
    } catch (e) {
      print('Erro: $e');
    }
  }
}
