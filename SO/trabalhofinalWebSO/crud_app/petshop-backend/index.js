// index.js
const express = require('express');
const { PrismaClient } = require('@prisma/client');

// --- Inicialização ---
const prisma = new PrismaClient();
const app = express();
app.use(express.json()); 
app.use((req, res, next) => {
  res.header('Access-Control-Allow-Origin', '*');
  res.header('Access-Control-Allow-Headers', 'Origin, X-Requested-With, Content-Type, Accept');
  if (req.method === 'OPTIONS') {
    res.header('Access-Control-Allow-Methods', 'GET, POST, PUT, DELETE, OPTIONS');
    return res.sendStatus(200);
  }
  next();
});
const PORT = 3000;

// ===== PETS (IGUAL A ANTES) =====
app.post('/pets', async (req, res) => {
  try {
    const { nome, raca, dono } = req.body;
    const pet = await prisma.pet.create({
      data: { nome, raca, dono },
    });
    res.status(201).json(pet);
  } catch (error) {
    res.status(400).json({ error: error.message });
  }
});

app.get('/pets', async (req, res) => {
  const pets = await prisma.pet.findMany();
  res.json(pets);
});
// ... (PUT e DELETE de pets são iguais) ...
app.put('/pets/:id', async (req, res) => {
  try {
    const { id } = req.params;
    const { nome, raca, dono } = req.body;
    const pet = await prisma.pet.update({
      where: { id: parseInt(id) },
      data: { nome, raca, dono },
    });
    res.json(pet);
  } catch (error) {
    res.status(404).json({ error: 'Pet não encontrado' });
  }
});

app.delete('/pets/:id', async (req, res) => {
  try {
    const { id } = req.params;
    await prisma.pet.delete({
      where: { id: parseInt(id) },
    });
    res.status(204).send();
  } catch (error) {
    res.status(404).json({ error: 'Pet não encontrado' });
  }
});


// ===== SERVIÇOS (IGUAL A ANTES) =====
app.post('/servicos', async (req, res) => {
  try {
    const { nome, preco } = req.body;
    const servico = await prisma.servico.create({
      data: { nome, preco: parseFloat(preco) },
    });
    res.status(201).json(servico);
  } catch (error) {
    res.status(400).json({ error: error.message });
  }
});

app.get('/servicos', async (req, res) => {
  const servicos = await prisma.servico.findMany();
  res.json(servicos);
});
// ... (PUT e DELETE de serviços são iguais) ...
app.put('/servicos/:id', async (req, res) => {
  try {
    const { id } = req.params;
    const { nome, preco } = req.body;
    const servico = await prisma.servico.update({
      where: { id: parseInt(id) },
      data: { nome, preco: preco ? parseFloat(preco) : undefined, },
    });
    res.json(servico);
  } catch (error) {
    res.status(404).json({ error: 'Serviço não encontrado' });
  }
});

app.delete('/servicos/:id', async (req, res) => {
  try {
    const { id } = req.params;
    await prisma.servico.delete({
      where: { id: parseInt(id) },
    });
    res.status(204).send();
  } catch (error) {
    res.status(404).json({ error: 'Serviço não encontrado' });
  }
});


// ===== AGENDAMENTOS (IGUAL A ANTES) =====
app.post('/agendamentos', async (req, res) => {
  try {
    const { dataHora, petId, servicoId } = req.body;
    const novoAgendamento = await prisma.agendamento.create({
      data: {
        dataHora: new Date(dataHora),
        petId: parseInt(petId),
        servicoId: parseInt(servicoId),
      },
      include: { pet: true, servico: true },
    });
    res.status(201).json(novoAgendamento);
  } catch (error) {
    res.status(400).json({ error: error.message });
  }
});

app.get('/agendamentos', async (req, res) => {
  const agendamentos = await prisma.agendamento.findMany({
    include: { pet: true, servico: true }, 
    orderBy: { dataHora: 'asc' }
  });
  res.json(agendamentos);
});
// ... (PUT e DELETE de agendamentos são iguais) ...
app.put('/agendamentos/:id', async (req, res) => {
  try {
    const { id } = req.params;
    const { dataHora, status } = req.body;
    const agendamento = await prisma.agendamento.update({
      where: { id: parseInt(id) },
      data: {
        dataHora: dataHora ? new Date(dataHora) : undefined,
        status: status,
      },
    });
    res.json(agendamento);
  } catch (error) {
    res.status(404).json({ error: 'Agendamento não encontrado' });
  }
});

app.delete('/agendamentos/:id', async (req, res) => {
  try {
    const { id } = req.params;
    await prisma.agendamento.delete({
      where: { id: parseInt(id) },
    });
    res.status(204).send();
  } catch (error) {
    res.status(404).json({ error: 'Agendamento não encontrado' });
  }
});


// ===== V A C I N A S (AGORA A USAR O PRISMA) =====

// GET /vacinas
app.get('/vacinas', async (req, res) => {
  try {
    const vacinas = await prisma.vacina.findMany({
      include: { pet: true }, // Isto vai "juntar" os dados do pet (incluindo o nome)
      orderBy: { dataAplicacao: 'desc' }
    });
    
    // O seu JS de vacinas (front-end) espera 'petNome'. 
    // Vamos formatar a resposta para que ela o inclua.
    const resultadoFormatado = vacinas.map(v => ({
      id: v.id,
      nomeVacina: v.nomeVacina,
      dataAplicacao: v.dataAplicacao,
      proximaDose: v.proximaDose,
      petId: v.petId,
      petNome: v.pet.nome // <- O nome do pet vindo do 'include'
    }));

    res.json(resultadoFormatado);
  } catch (err) {
    res.status(500).json({ error: err.message });
  }
});

// POST /vacinas (CORRIGIDO PARA O PRISMA)
app.post('/vacinas', async (req, res) => {
  try {
    // 1. Agora lemos petId (o front-end está a enviar isto)
    const { petId, nomeVacina, dataAplicacao, proximaDose } = req.body;
    
    // 2. A validação correta
    if (!petId || !nomeVacina || !dataAplicacao) {
      return res.status(400).json({ error: 'Campos obrigatórios: petId, nomeVacina, dataAplicacao' });
    }

    // 3. Criamos o registo na tabela 'Vacina' usando o Prisma
    const novaVacina = await prisma.vacina.create({
      data: {
        petId: parseInt(petId),
        nomeVacina: nomeVacina,
        dataAplicacao: new Date(dataAplicacao),
        proximaDose: proximaDose ? new Date(proximaDose) : null
      }
    });

    res.status(201).json(novaVacina);
  } catch (err) {
    res.status(500).json({ error: err.message });
  }
});

// DELETE /vacinas/:id (CORRIGIDO PARA O PRISMA)
app.delete('/vacinas/:id', async (req, res) => {
  try {
    const { id } = req.params;
    
    // 1. Apagamos da tabela 'Vacina' usando o Prisma
    await prisma.vacina.delete({
      where: { id: parseInt(id) }
    });

    res.status(204).send();
  } catch (err) {
    res.status(500).json({ error: err.message });
  }
});


// servidor 
app.listen(PORT, () => {
  console.log(`🚀 Servidor rodando em http://localhost:${PORT}`);
});