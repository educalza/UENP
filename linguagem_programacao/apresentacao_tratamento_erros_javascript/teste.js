function dividir (a , b ) {
    if(b === 0) throw new Error("Divisão por zero!");
    return a / b;
}
try {
    console.log(dividir(10, 2)); // deve funcionar
    console.log(dividir(5, 0)); // deve lançar erro
} catch (error) {
    console.error("Erro ao dividir:", error.message);   
}


