package com.meuteste;

import javafx.fxml.FXML;
import javafx.scene.control.TextField;
import javafx.scene.control.Button;

public class TelaController {
    
    @FXML
    private TextField saidaEstoque;
    @FXML
    private TextField entradaAbastecer;
    @FXML
    private TextField entradaVenderQtd;
    
    private MaquinaCoxinha maquinaCoxinha;
    
    @FXML
    private void initialize(){
    maquinaCoxinha = new MaquinaCoxinha();
    }
    
    private void atualizarEstoque() {
        saidaEstoque.setText(String.valueOf(maquinaCoxinha.getEstoque()));
    }
    
    @FXML
    private void abastecer() {
            int quantidade = Integer.parseInt(entradaAbastecer.getText());
            maquinaCoxinha.abastecer(quantidade);
            atualizarEstoque();
    }
    
    @FXML
    private void venderUmaCoxinha() {
        maquinaCoxinha.vender();
        atualizarEstoque();
    }
    
    @FXML
    private void venderQtd() {
            int quantidade = Integer.parseInt(entradaVenderQtd.getText());
            maquinaCoxinha.vender(quantidade);
            atualizarEstoque();
    }
    
    @FXML
    private void zerarMaquina() {
        maquinaCoxinha.zerar();
        atualizarEstoque();
    }
}