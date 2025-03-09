package trabalho1;

public class SpamDetector {

    public static final String[][] mat = {
            {"Você ganhou", "8"},
            {"Tentativa de entrega frustrada", "4"},
            {"Clique aqui", "9"},
            {"Oferta imperdível", "7"},
            {"Parabéns", "6"},
            {"Ação necessária", "5"},
            {"Atualização da sua conta", "5"},
            {"Ganhe dinheiro rápido", "8"},
            {"Seu prêmio aguarda", "7"},
            {"Oferta limitada", "6"}
    };

    public static String calcularSpam(String str){

        int peso = 0;

        // :
        for(String[] aux: mat){
            String var = aux[0];
            int p = Integer.parseInt(aux[1]);
            if(str.toLowerCase().contains(var.toLowerCase())){
                peso += p;
            }
        }
        if(peso>=5&&peso<=30){
            return "Fracamente suspeito " + peso;
        }
        else if(peso>=31&&peso<=70){
            return "Potencialmente suspeito " + peso;
        }
        else if(peso>70){
            return "Spam " + peso;
        }
        return "Não é Spam " + peso;
    }
}

