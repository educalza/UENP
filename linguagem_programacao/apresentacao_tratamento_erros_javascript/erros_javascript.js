//SyntaxError

// Erro de sintaxe 
console.log("Olá";



//ReferenceError

// Variável não definida
console.log(nome); 
// nome nunca foi definido


// TypeError

// Tentativa de acessar uma propriedade de um valor não-objeto
let pessoa = null;
console.log(pessoa.nome); 
// TypeError: Cannot read property 'nome' of null


try {
  console.log(dividir(10, 0));
} catch (e) {
  console.log("Erro:", e.message);
}