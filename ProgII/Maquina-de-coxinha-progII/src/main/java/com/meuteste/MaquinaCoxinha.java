package com.meuteste;

public class MaquinaCoxinha {
    private int estoque;
    
    public MaquinaCoxinha(){
        this.estoque = 0;
    }
    
    public int getEstoque(){
        return estoque;
    }
    
    public void abastecer(int qtd){
        if (qtd > 0) {
            estoque += qtd;
        }
    }

    public void vender(){
        if(estoque > 0){
            estoque--;
        }
    }
    
    public void vender(int qtd) {
        if (qtd > 0 && qtd <= estoque) {
            estoque -= qtd;
        }
    }
    
    public void zerar() {
        estoque = 0;
    }
}
