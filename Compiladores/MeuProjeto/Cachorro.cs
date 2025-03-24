using System;

class Cachorro : Animal
{
    public Cachorro(string nome) : base(nome) {}

    // Sobrescrevendo o método EmitirSom (polimorfismo)
    public override void EmitirSom()
    {
        Console.WriteLine($"{Nome} (Cachorro) late: Au Au!");
    }
}