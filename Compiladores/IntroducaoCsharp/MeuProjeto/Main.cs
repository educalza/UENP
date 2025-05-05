using System;

class Program
{
    static void Main()
    {
        // Criando uma lista de animais
        Animal[] animais = new Animal[]
        {
            new Cachorro("Rex"),
            new Gato("Mia"),
            new Cachorro("Thor")
        };

        // Chamando o método polimórfico
        foreach (var animal in animais)
        {
            animal.EmitirSom();
        }
    }
}