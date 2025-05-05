package com;

import java.io.IOException;
import java.util.List;
import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.fxml.FXML;
import javafx.scene.control.Alert;
import javafx.scene.control.Alert.AlertType;
import javafx.scene.control.ComboBox;
import modelo.Vaca;
import util.Dao;

public class VacaExcluirController {
    
    @FXML
    private ComboBox<String> cbVacas;
    
    private Dao<Vaca> daoVacas;
    private ObservableList<Vaca> listaVacas;
    
    @FXML
    private void initialize(){
        daoVacas = new Dao<>(Vaca.class);
        carregarVacas();
    }
    
    @FXML
    private void cancelar() throws IOException{
        App.setRoot("Menu");
    }
    
    @FXML
    private void excluir(){
        String brincoVaca = cbVacas.getValue();
        
        if(brincoVaca == null){
            mostrarAlerta(AlertType.WARNING, "AVISO", "Erro ao selecionar brinco.");
            return;
        }
        
        Vaca vaca = daoVacas.buscarPorChave("brinco", brincoVaca);
        
        if(vaca == null){
            mostrarAlerta(AlertType.WARNING, "AVISO","A vaca não existe");
            return;
        }
        
        boolean sucess = daoVacas.excluir("brinco", brincoVaca);
        
        if(sucess){
            mostrarAlerta(AlertType.INFORMATION, "SUCESSO", "Vaca excluida com sucesso");
            carregarVacas();
        } else{
            mostrarAlerta(AlertType.ERROR, "ERRO", "Erro desconhecido");
        }
    }
    
    @FXML
    private void carregarVacas(){
        List<Vaca> vacas = daoVacas.listarTodos();
        ObservableList<String> brincos = FXCollections.observableArrayList();
        
        if(vacas.isEmpty()){
            cbVacas.setPromptText("Nenhuma vaca");
            cbVacas.setItems(FXCollections.observableArrayList()); // Limpa a lista
        } else{
            for(Vaca vaca : vacas){
                brincos.add(vaca.getBrinco());
            }
            cbVacas.setItems(brincos);
            cbVacas.setPromptText("Selecione uma vaca");    
        }
    }
    
    private void mostrarAlerta(Alert.AlertType tipo, String titulo, String mensagem){
        Alert alert = new Alert(tipo);
        alert.setTitle(titulo);
        alert.setHeaderText(null);
        alert.setContentText(mensagem);
        alert.showAndWait();
    }
}
