function esperar(ms) {
  return new Promise(resolve => setTimeout(resolve, ms));
}

async function contador() {
  for (let i = 1; i <= 5; i++) {
    console.log(i);
    await esperar(1000); // espera 1 segundo entre os números
  }
  console.log("Contagem concluída!");
}

contador();
