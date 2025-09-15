public class Main {

	public static void main(String[] args) {
		Emprestimo e = new Emprestimo();

		Livro l1 = new Livro(1);
		
		Livro l2 = new Livro(2);
		
		 Livro l3 = new Livro(3); 
		
		Item i1 = new Item(l1);
		Item i2 = new Item(l2);
		Item i3 = new Item(l3);
		e.i.add(i1);
		e.i.add(i2);
		e.i.add(i3);

		System.out.print("Data de Empréstimo: " + e.getDataEmprestimo());
		System.out.print("\nData de Devolução: " + e.CalculaDataDevolucao());

	}

}