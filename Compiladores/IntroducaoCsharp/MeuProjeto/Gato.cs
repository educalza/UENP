using System;

class Gato : Animal
{
    private string Raca { get; set; }

    public Gato(string nome, string raca) : base(nome) {
        Raca = raca
    }

    // Sobrescrevendo o método EmitirSom (polimorfismo)
    public override void EmitirSom()
    {
        Console.WriteLine($"{Nome} (Gato -  " + raca + ") mia: Miau!");
    }
}