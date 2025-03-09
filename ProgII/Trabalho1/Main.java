package trabalho1;

public class Main {

    public static void main(String[] args){
        LeitorArquivo leitorarquivo = new LeitorArquivo();

        String str = leitorarquivo.lerArquivo("C:\\Users\\eduar\\IdeaProjects\\Trabalho1\\Spam.txt");

        System.out.println(SpamDetector.calcularSpam(str));
    }
}
