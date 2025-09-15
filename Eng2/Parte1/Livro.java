
public class Livro {
	Titulo titulo;
	public Livro(int codigo) {
		//instância um titulo e o associa ao livro
		titulo = new Titulo(codigo);
	}
	public int verPrazo() {
		return titulo.getPrazo();
	}

	
}

