let total = 0;
for (let i = 1; i <= 1_000_000_000; i++) {
  total += i;

  // A cada 100 milhões, envia uma atualização
  if (i % 100_000_000 === 0) {
    postMessage(`Contando... já somamos até ${i}`);
  }
}

postMessage(`Contagem concluída! Soma total: ${total}`);
