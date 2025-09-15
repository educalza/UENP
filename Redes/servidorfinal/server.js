// server.js

const express = require('express');
const http = require('http');
const path = require('path');
const WebSocket = require('ws'); // Importa a biblioteca WebSocket

const app = express();
const server = http.createServer(app);
const wss = new WebSocket.Server({ server }); // Cria o servidor WebSocket

const PORT = process.env.PORT || 3000;

// Configura o Express para servir arquivos estáticos da pasta 'public'
app.use(express.static(path.join(__dirname, 'public')));

// Rota principal (opcional, já que estamos servindo a pasta estática)
app.get('/', (req, res) => {
    res.sendFile(path.join(__dirname, 'public', 'index.html'));
});

// Eventos do WebSocket
wss.on('connection', ws => {
    console.log('Novo cliente conectado!');

    // Lida com mensagens recebidas do cliente (navegador ou ESP32)
    ws.on('message', message => {
        console.log('Comando recebido: %s', message);

        // Retransmite a mensagem para todos os clientes conectados (incluindo o ESP32)
        wss.clients.forEach(client => {
            if (client.readyState === WebSocket.OPEN) {
                client.send(message.toString());
            }
        });
    });

    ws.on('close', () => {
        console.log('Cliente desconectado.');
    });
});

// Inicia o servidor
server.listen(PORT, () => {
    console.log(`Servidor rodando em http://localhost:${PORT}`);
});