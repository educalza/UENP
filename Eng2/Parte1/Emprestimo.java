import java.util.ArrayList;
import java.util.Calendar;
import java.util.Date;
import java.util.List;

public class Emprestimo {

	Date dataEmprestimo = new Date();
	/*metodo para gegar a variavel dataEmprestimo*/
	public Date getDataEmprestimo() {
		return dataEmprestimo;
	}

	/*metodo para setar a variavel dataEmprestimo*/
	public void setDataEmprestimo(Date dataEmprestimo) {
		this.dataEmprestimo = dataEmprestimo;
	}

	/*utilize essas v�riaveis para calcular a data final de devolu��o*/
        Date dataPrevista = new Date();
	Date data_aux = new Date();
	String nome;

        /*Cada Emprestimo � composto de v�rios itens*/
	List<Item> i = new ArrayList<Item>();
	
        //Metodo respons�vel por calcular a data de devolu��o
	public Date CalculaDataDevolucao()
	{   
		Date date = new Date();
		/*Voc� precisa implementar as regras do c�lculo da devolu��o dentro deste m�todo*/

                /*N�o esque�a de chamar o m�todo para calcular a data de devolu��o de cada item*/	

                //i.get(j).calculaDataDevolucao(date);

        	//Dica use o calendar para calcular as data
	 	Calendar calendar = Calendar.getInstance();
			
		
		return dataPrevista;
	}
	
}