function tarefaDemorada(ms, nome) {
  return new Promise(resolve => {
    setTimeout(() => {
      console.log(`Tarefa ${nome} concluída`);
      resolve();
    }, ms);
  });
}

async function executar() {
  console.log('Início');
  tarefaDemorada(2000, 'A');
  tarefaDemorada(1000, 'B');
  console.log('Fim');
}

executar();