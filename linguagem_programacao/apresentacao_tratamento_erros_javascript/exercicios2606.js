let nomes = ["Eduardo", "Paulo", "Isabela", "Ana", "Giovana", "Juliana","Giovanna", "Iogr",
     "vitinho", "Zé", "Zézinho", "Zezão", "Ozeias", "Danilo", "Diego", "gabriel", "pedro",
    "rafael", "miguel", "felipe", "Gustav"];

console.log(nomes);

nomes[0] = "Eduardinho";
nomes[1] = "Paulinho";
nomes[19] = "Gustavinho";
console.log(nomes);


console.log(new Date(2025, 5, 1));

console.log(new Date() - new Date());


let dataString = "2025-06-26T14:00:00";
let dataMilissegundos = Date.parse(dataString);
let novaData = new Date(dataMilissegundos);
console.log(novaData);

