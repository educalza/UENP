# Como Instalar e Configurar C# no Visual Studio Code

Este guia mostrará como instalar e configurar o ambiente para desenvolvimento em C# no Visual Studio Code (VS Code).

---

## 📌 Requisitos

Antes de começar, certifique-se de ter os seguintes requisitos instalados:

- [Visual Studio Code](https://code.visualstudio.com/download)
- [.NET SDK (Software Development Kit)](https://dotnet.microsoft.com/en-us/download/dotnet)

---

## 📥 Passo 1: Instalar o .NET SDK

O .NET SDK é necessário para compilar e executar programas em C#.

1. Acesse o site oficial da Microsoft para baixar o .NET SDK:  
   👉 [Download do .NET SDK](https://dotnet.microsoft.com/en-us/download/dotnet)
2. Escolha a versão mais recente recomendada.
3. Instale o SDK seguindo as instruções do instalador.
4. Para verificar se a instalação foi bem-sucedida, abra o terminal ou prompt de comando e execute:

   ```sh
   dotnet --version
   ```

   Se a instalação foi concluída com sucesso, ele exibirá a versão do .NET instalada.

---

## 🔧 Passo 2: Instalar a Extensão C# no VS Code

Para obter suporte completo ao C# no VS Code, instale a extensão oficial da Microsoft:

1. Abra o VS Code.
2. Vá até a aba de **Extensões** (`Ctrl + Shift + X`).
3. Pesquise por **"C#"**.
4. Instale a extensão oficial **C# (by Microsoft)**.

---

## 📂 Passo 3: Criar e Rodar um Projeto C#

### Criando um Novo Projeto C#
1. Abra o terminal no VS Code (`Ctrl + ~`).
2. Execute o seguinte comando para criar um novo projeto do tipo console:

   ```sh
   dotnet new console -o MeuProjeto
   ```

3. Acesse a pasta do projeto:

   ```sh
   cd MeuProjeto
   ```

### Executando o Código
1. No VS Code, abra a pasta do projeto (`Arquivo > Abrir Pasta`).
2. No terminal, execute o seguinte comando para compilar e rodar o programa:

   ```sh
   dotnet run
   ```

   Você verá a saída do programa no terminal.

---

## ⚙️ Configuração Extra: Habilitar o OmniSharp

Caso enfrente problemas com IntelliSense ou depuração, ative o **OmniSharp**:

1. No VS Code, pressione `Ctrl + Shift + P` e procure por **OmniSharp: Select Project**.
2. Selecione o arquivo `.csproj` do seu projeto.
3. Reinicie o VS Code se necessário.

---

## ✅ Conclusão
Agora você tem um ambiente configurado para desenvolver em C# no VS Code! 🚀

Caso tenha dúvidas, consulte a documentação oficial do .NET:  
👉 [Documentação do .NET](https://learn.microsoft.com/en-us/dotnet/)

