package com;

import java.io.IOException;
import java.time.LocalDate;
import java.util.List;
import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.fxml.FXML;
import javafx.scene.control.Alert;
import javafx.scene.control.Alert.AlertType;
import javafx.scene.control.ComboBox;
import javafx.scene.control.DatePicker;
import javafx.scene.control.TextField;
import modelo.Producao;
import modelo.Vaca;
import util.Dao;

public class CadastrarProducaoLeiteController {
    
    @FXML
    private ComboBox<String> cbVaca;
    
    @FXML
    private DatePicker dpData;
    
    @FXML
    private TextField txtProducaoLitros;
    
    private Dao<Producao> daoProducao;
    private Dao<Vaca> daoVaca;
    
    @FXML
    private void initialize(){
        daoProducao = new Dao<>(Producao.class);
        daoVaca = new Dao<>(Vaca.class);
        carregarVacas();
    }
    
    @FXML
    private void cancelar() throws IOException{
        App.setRoot("Menu");
    }
    
    @FXML
    private void cadastrar(){
        try{
            String brinco = cbVaca.getValue();
            LocalDate data = dpData.getValue();
            double quantidade;
            
            if(brinco.isBlank() || data == null || txtProducaoLitros.getText().isBlank()){
                mostrarAlerta(AlertType.WARNING, "AVISO", "Preencha todos os campos.");
                return;
            }
            
            try{
                quantidade = Double.parseDouble(txtProducaoLitros.getText());
                if(quantidade <= 0){
                    mostrarAlerta(AlertType.WARNING, "AVISO", "A quantidade deve ser maior que zero.");
                    return;
                }
            } catch(NumberFormatException e){
                mostrarAlerta(AlertType.WARNING, "AVISO", "Digite um valor válido.");
                return;
            }
            
            Vaca vaca = daoVaca.buscarPorChave("brinco", brinco);
            if(vaca == null){
                mostrarAlerta(AlertType.WARNING, "AVISO", "Vaca inexistente.");
                return;
            }
            
            Producao producao = new Producao(brinco, data, quantidade);
            daoProducao.inserir(producao);
            
            mostrarAlerta(Alert.AlertType.INFORMATION, "SUCESSO", "Produção cadastrada com sucesso.");
            limpar();
        } catch (Exception e){
            mostrarAlerta(AlertType.ERROR, "ERRO", "Erro desconhecido " + e.getMessage());
        }
    }
    
    @FXML
    private void carregarVacas(){
        List<Vaca> vacas = daoVaca.listarTodos();
        ObservableList<String> brincos = FXCollections.observableArrayList();
        
        if(vacas.isEmpty()){
            cbVaca.setPromptText("Nenhuma vaca");
            cbVaca.setItems(FXCollections.observableArrayList()); // Limpa a lista
        } else{
            for(Vaca vaca : vacas){
                brincos.add(vaca.getBrinco());
            }
            cbVaca.setItems(brincos);
            cbVaca.setPromptText("Selecione uma vaca");    
        }
    }
    
    @FXML
    private void limpar(){
        carregarVacas();
        dpData.setValue(null);
        txtProducaoLitros.clear();
    }
    
    private void mostrarAlerta(Alert.AlertType tipo, String titulo, String mensagem){
        Alert alert = new Alert(tipo);
        alert.setTitle(titulo);
        alert.setHeaderText(null);
        alert.setContentText(mensagem);
        alert.showAndWait();
    }
}
