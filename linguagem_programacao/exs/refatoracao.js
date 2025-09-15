const price = 100;
const discount = 0.2;

function calculateTotal() {

  const total = price - price * discount;
  return total;
}


function printMessage(user = "Guest", message = "Welcome!") {
  console.log(`User: ${user} - ${message}`);
}


const car = { model: "Fiat", year: 2020 };

car.status = "broken";


const x = 5 + Number("5");

console.log(calculateTotal());
printMessage();
console.log("Valor de x:", x);
console.log("Carro:", car);
