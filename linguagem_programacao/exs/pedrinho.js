/*Utilizando a API fetch pegue o nome de
 todos os pokemons tipo fogo */

fetch("https://pokeapi.co/api/v2/type/fire")
  .then(response => response.json())
  .then(data => {
    const pokemons = data.pokemon.map(p => p.pokemon.name);
    console.log("Pokémons do tipo fogo:", pokemons);
  })
  .catch(error => console.error("Erro ao buscar os Pokémons:", error));

