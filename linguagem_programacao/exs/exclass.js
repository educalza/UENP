class Carro{
    constructor(marca, modelo, ano){
        this.marca = marca;
        this.modelo = modelo;
        this.ano = ano;
    }

    ExibirInfo(){
        console.log(`Carro: ${this.marca} ${this.modelo}, Ano: ${this.ano}`);
    }
}


const carro1 = new Carro('Toyota', 'Corolla', 2020);
const carro2 = new Carro('Honda', 'Civic', 2021);

carro1.ExibirInfo();
carro2.ExibirInfo();