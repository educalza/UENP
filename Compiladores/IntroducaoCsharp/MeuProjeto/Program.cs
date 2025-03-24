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

// Classe base
class Animal
{
    public string Nome { get; set; }

    public Animal(string nome)
    {
        Nome = nome;
    }

    // Método virtual para ser sobrescrito nas classes derivadas
    public virtual void EmitirSom()
    {
        Console.WriteLine("O animal faz um som.");
    }
}

// Classe derivada Cachorro
class Cachorro : Animal
{
    public Cachorro(string nome) : base(nome) {}

    // Sobrescrevendo o método EmitirSom (polimorfismo)
    public override void EmitirSom()
    {
        Console.WriteLine($"{Nome} (Cachorro) late: Au Au!");
    }
}

// Classe derivada Gato
class Gato : Animal
{
    public Gato(string nome) : base(nome) {}

    // Sobrescrevendo o método EmitirSom (polimorfismo)
    public override void EmitirSom()
    {
        Console.WriteLine($"{Nome} (Gato) mia: Miau!");
    }
}
