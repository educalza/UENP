const botao = document.getElementById("iniciar");
const status = document.getElementById("status");
let worker;

botao.addEventListener("click", () => {
  status.textContent = "Contando até 1 bilhão...";
  
  worker = new Worker("worker.js");

  worker.onmessage = function (e) {
    status.textContent = e.data;
  };

  worker.onerror = function (e) {
    console.error("Erro no Worker:", e.message);
    status.textContent = "Erro na contagem!";
  };
});
