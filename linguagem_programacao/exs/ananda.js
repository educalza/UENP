const alunos = ['Ana', 'Carlos', 'Joao', 'Ana', 'Carlos', 'Lucas'];

const presencas = new Map([
  ['Workshop JS', new Set(['Ana', 'Carlos', 'Joao'])],
  ['Workshop Python', new Set(['Carlos', 'Lucas'])],
  ['Workshop Java', new Set(['Joao', 'Ana'])],
]);

// 1. removendo duplicatas
const alunosUnicos = new Set(alunos);

// 2. criando map
const participacoes = new Map();

for (const aluno of alunosUnicos) {
  const workshopsDoAluno = new Set();

  for (const [workshop, participantes] of presencas.entries()) {
    if (participantes.has(aluno)) {
      workshopsDoAluno.add(workshop);
    }
  }

  participacoes.set(aluno, workshopsDoAluno);
}

// 3. imprimir
for (const [aluno, workshops] of participacoes.entries()) {
  console.log(`${aluno}: ${[...workshops].join(', ')}`);
}
