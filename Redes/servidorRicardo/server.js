// server.js
const http = require("http");
const fs = require("fs");
const path = require("path");
const WebSocket = require("ws");
const url = require("url");

// Carregar usuários do arquivo JSON
let users = {};
try {
  const usersData = fs.readFileSync(path.join(__dirname, "users.json"), "utf8");
  users = JSON.parse(usersData);
} catch (err) {
  console.error("Erro ao carregar usuários:", err);
}

// Servidor HTTP para servir páginas e API
const server = http.createServer((req, res) => {
  const parsedUrl = url.parse(req.url, true);
  const pathname = parsedUrl.pathname;

  // CORS headers
  res.setHeader('Access-Control-Allow-Origin', '*');
  res.setHeader('Access-Control-Allow-Methods', 'GET, POST, OPTIONS');
  res.setHeader('Access-Control-Allow-Headers', 'Content-Type');

  // Rota de login
  if (pathname === "/login" && req.method === "POST") {
    let body = "";
    req.on("data", chunk => {
      body += chunk.toString();
    });
    req.on("end", () => {
      try {
        const { username, password } = JSON.parse(body);
        
        if (users[username] && users[username] === password) {
          res.writeHead(200, { "Content-Type": "application/json" });
          res.end(JSON.stringify({ success: true, message: "Login realizado com sucesso" }));
        } else {
          res.writeHead(401, { "Content-Type": "application/json" });
          res.end(JSON.stringify({ success: false, message: "Usuário ou senha incorretos" }));
        }
      } catch (err) {
        res.writeHead(400, { "Content-Type": "application/json" });
        res.end(JSON.stringify({ success: false, message: "Dados inválidos" }));
      }
    });
    return;
  }

  // Servir arquivos estáticos
  let filePath;
  if (pathname === "/") {
    filePath = path.join(__dirname, "index.html");
  } else if (pathname === "/carros.html") {
    filePath = path.join(__dirname, "carros.html");
  } else if (pathname === "/controle.html") {
    filePath = path.join(__dirname, "controle.html");
  } else {
    filePath = path.join(__dirname, pathname);
  }

  // Verificar se arquivo existe
  fs.access(filePath, fs.constants.F_OK, (err) => {
    if (err) {
      res.writeHead(404, { "Content-Type": "text/html" });
      res.end(`
        <html>
          <body>
            <h1>404 - Página não encontrada</h1>
            <p>A página solicitada não existe.</p>
            <a href="/">Voltar ao início</a>
          </body>
        </html>
      `);
      return;
    }

    // Determinar tipo de conteúdo
    const ext = path.extname(filePath);
    let contentType = "text/html";
    
    if (ext === ".js") contentType = "application/javascript";
    else if (ext === ".css") contentType = "text/css";
    else if (ext === ".json") contentType = "application/json";
    else if (ext === ".png") contentType = "image/png";
    else if (ext === ".jpg" || ext === ".jpeg") contentType = "image/jpeg";

    fs.readFile(filePath, (err, content) => {
      if (err) {
        res.writeHead(500);
        res.end("Erro interno do servidor");
      } else {
        res.writeHead(200, { "Content-Type": contentType });
        res.end(content);
      }
    });
  });
});

// WebSocket rodando no mesmo servidor HTTP
const wss = new WebSocket.Server({ server });

// Mapas para guardar conexões
let cars = {};   // { carId: { ws, streamUrl } }
let connectedUsers = {};  // { userId: { ws, carId } }

wss.on("connection", (ws) => {
  console.log("Nova conexão recebida");

  ws.on("message", (msg) => {
    try {
      const data = JSON.parse(msg);

      // ESP32 se conectando
      if (data.type === "register_car") {
        const { carId, streamUrl } = data;
        cars[carId] = { ws, streamUrl };
        ws.carId = carId;

        console.log(`🚗 Carro registrado: ${carId} (${streamUrl})`);
        console.log(`📊 Total de carros conectados: ${Object.keys(cars).length}`);

        ws.send(JSON.stringify({ type: "registered", carId }));
      }

      // Usuário do browser conectando
      else if (data.type === "register_user") {
        const { userId } = data;
        connectedUsers[userId] = { ws, carId: null };
        ws.userId = userId;

        const carsInfo = Object.entries(cars).map(([id, car]) => ({
          carId: id,
          streamUrl: car.streamUrl,
        }));

        console.log(`🙋 Usuário registrado: ${userId}`);

        ws.send(JSON.stringify({ type: "registered", userId, cars: carsInfo }));
      }

      // Usuário seleciona carro
      else if (data.type === "select_car") {
        const { userId, carId } = data;

        if (!cars[carId]) {
          ws.send(JSON.stringify({ type: "error", message: "Carro não disponível" }));
          return;
        }

        // Verifica se já está sendo usado
        const inUse = Object.values(connectedUsers).some((u) => u.carId === carId);
        if (inUse) {
          ws.send(JSON.stringify({ type: "error", message: "Carro já está em uso" }));
          return;
        }

        connectedUsers[userId].carId = carId;
        console.log(`✅ Usuário ${userId} conectado ao carro ${carId}`);

        ws.send(JSON.stringify({ type: "car_selected", carId }));
      }

      // Usuário manda comando para o carro
      else if (data.type === "command") {
        const { userId, command } = data;
        const carId = connectedUsers[userId]?.carId;

        if (carId && cars[carId]) {
          const commandData = { type: "command", command };
          cars[carId].ws.send(JSON.stringify(commandData));
          console.log(`🎮 Comando enviado para ${carId}: ${command} (usuário: ${userId})`);
        } else if (carId && !cars[carId]) {
          console.log(`❌ Erro: Carro ${carId} não está mais conectado (usuário: ${userId})`);
          // Notificar o usuário que o carro foi desconectado
          const userWs = connectedUsers[userId]?.ws;
          if (userWs) {
            userWs.send(JSON.stringify({ 
              type: "error", 
              message: `Carro ${carId} foi desconectado` 
            }));
          }
        } else {
          console.log(`❌ Erro: Usuário ${userId} tentou enviar comando ${command} mas não está conectado a um carro`);
        }
      }

      // Usuário manda comando analógico do controle
      else if (data.type === "analog_command") {
        const { userId, x, y } = data;
        const carId = connectedUsers[userId]?.carId;

        if (carId && cars[carId]) {
          const analogData = { type: "analog_command", x, y };
          cars[carId].ws.send(JSON.stringify(analogData));
          console.log(`🎮 Comando analógico enviado para ${carId}: X=${x.toFixed(2)}, Y=${y.toFixed(2)} (usuário: ${userId})`);
        } else if (carId && !cars[carId]) {
          console.log(`❌ Erro: Carro ${carId} não está mais conectado (usuário: ${userId})`);
          // Notificar o usuário que o carro foi desconectado
          const userWs = connectedUsers[userId]?.ws;
          if (userWs) {
            userWs.send(JSON.stringify({ 
              type: "error", 
              message: `Carro ${carId} foi desconectado` 
            }));
          }
        } else {
          console.log(`❌ Erro: Usuário ${userId} tentou enviar comando analógico mas não está conectado a um carro`);
        }
      }

      // Carro manda status
      else if (data.type === "status") {
        const { carId, status } = data;
        const user = Object.values(connectedUsers).find((u) => u.carId === carId);

        if (user) {
          user.ws.send(JSON.stringify({ type: "status", carId, status }));
        }
      }
    } catch (err) {
      console.error("Erro ao processar mensagem:", err);
    }
  });

  ws.on("close", () => {
    if (ws.carId) {
      console.log(`🚗 Carro desconectado: ${ws.carId}`);
      delete cars[ws.carId];
      console.log(`📊 Total de carros conectados: ${Object.keys(cars).length}`);
    }
    if (ws.userId) {
      console.log(`🙋 Usuário desconectado: ${ws.userId}`);
      delete connectedUsers[ws.userId];
      console.log(`📊 Total de usuários conectados: ${Object.keys(connectedUsers).length}`);
    }
  });
});

const PORT = process.env.PORT || 8080;

server.listen(PORT, () => {
  console.log(`🚀 Servidor rodando na porta ${PORT}`);

  // Se estiver no Heroku, imprime a URL pública
  if (process.env.NODE_ENV === "production") {
    console.log("🌐 Acesse em: " + (process.env.HEROKU_APP_NAME 
      ? `https://${process.env.HEROKU_APP_NAME}.herokuapp.com`
      : "URL do Heroku"));
  } else {
    console.log(`🔗 Local: http://localhost:${PORT}`);
  }
});