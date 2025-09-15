
public class Titulo {
int prazo;
public Titulo(int codigo)
{
	//Dá o prazo de devolução como sendo o código do livro mais 1
 	this.prazo = codigo+1;
}
public int getPrazo() {
	return prazo;
}

public void setPrazo(int prazo) {
	this.prazo = prazo;
}

}
