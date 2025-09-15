try {
  // código que pode gerar erro
} catch (erro) {
  // tratamento do erro
}



try {
  let resultado = 10 / 0;
  console.log(resultado);
} catch (e) {
  console.log("Erro:", e.message);
}


try {
  // algo que pode dar erro
} catch (e) {
  // erro tratado
} finally {
  console.log("Finalizando operação");
}



function dividir(a, b) {
  if (b === 0) throw new Error("Divisão por zero!");
  return a / b;
}
