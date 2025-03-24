using System;

class Animal
{
    protected string Nome { get; set; }

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