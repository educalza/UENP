using System;
using System.IO;

class Program
{
    static void Main()
    {
        // O arquivo é aberto e fechado automaticamente após o bloco using
        using (StreamReader leitor = new StreamReader("arquivo.txt"))
        {
            string conteudo = leitor.ReadToEnd();
            Console.WriteLine(conteudo);
        } // Aqui, o método Dispose() do StreamReader é chamado automaticamente.

        Console.WriteLine("O arquivo foi fechado automaticamente!");
    }
}