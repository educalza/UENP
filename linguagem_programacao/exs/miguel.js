const resposta = await fetch("https://pokeapi.co/api/v2/type/fire");
const dados = await resposta.json();
const nomes = dados.pokemon.map(p => p.pokemon.name);
console.log(nomes);
function processar(num1, num2, callback) {
  const resultado = callback(num1, num2);
  console.log(resultado);
}


function somar(a, b) {
  return a + b;
}

function multiplicar(a, b) {
  return a * b;
}

processar(3, 4, somar);       // Imprime: 7
processar(3, 4, multiplicar); // Imprime: 12
