package com;

import java.io.IOException;
import java.util.List;
import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.fxml.FXML;
import javafx.scene.control.Alert;
import javafx.scene.control.ComboBox;
import javafx.scene.control.TextField;
import modelo.Vaca;
import util.Dao;

public class VacaEditarController {
    
    @FXML
    private ComboBox<String> cbVaca;
    
    @FXML private TextField txtNovoBrinco;
    @FXML private TextField txtNovoNome;
    @FXML private TextField txtNovaRaca;
    
    private Dao<Vaca> daoVaca;
    private Vaca vaca;
    private ObservableList<Vaca> listaVaca;
    
    @FXML
    private void initialize(){
        daoVaca = new Dao<>(Vaca.class);
        carregarVacas();
    }
    
    @FXML
    private void comboBox(){
        String brincoEditar = cbVaca.getValue();
        
        if (brincoEditar == null || brincoEditar.isBlank()) {
            return;
        }
        
        vaca = daoVaca.buscarPorChave("brinco", brincoEditar);
        if(vaca == null){
            mostrarAlerta(Alert.AlertType.WARNING, "AVISO", "A vaca não existe.");
            return;
        }
        
        txtNovoBrinco.setText(vaca.getBrinco());
        txtNovoNome.setText(vaca.getNome());
        txtNovaRaca.setText(vaca.getRaca());
    }
    
    @FXML
    private void confirmar(){
        if(vaca == null){
            mostrarAlerta(Alert.AlertType.WARNING, "AVISO", "Erro ao selecionar login");
            return;
        }
        
        String novoBrinco = txtNovoBrinco.getText();
        String novoNome = txtNovoNome.getText();
        String novaRaca = txtNovaRaca.getText();
        
        if(novoBrinco.isBlank() || novoNome.isBlank() || novaRaca.isBlank()){
            mostrarAlerta(Alert.AlertType.WARNING, "AVISO", "Preencha todos os campos");
            return;
        }
        
        Vaca vacaExistente = daoVaca.buscarPorChave("brinco", novoBrinco);
        if(vacaExistente != null && !vacaExistente.getBrinco().equals(vaca.getBrinco())){
            mostrarAlerta(Alert.AlertType.WARNING, "AVISO", "Brinco existente");
            return;
        }
        
        vaca.setBrinco(novoBrinco);
        vaca.setNome(novoNome);
        vaca.setRaca(novaRaca);
        
        daoVaca.alterar("brinco", cbVaca.getValue().toUpperCase(), vaca);
        mostrarAlerta(Alert.AlertType.INFORMATION, "SUCESSO", "Vaca alterado com sucesso");
        carregarVacas();
        limpar();
    }
    
    private void carregarVacas(){
        List<Vaca> vacas = daoVaca.listarTodos();
        ObservableList<String> logins = FXCollections.observableArrayList();
        for(Vaca vaca : vacas){
            logins.add(vaca.getBrinco());
        }
        cbVaca.setItems(logins);
    }
    
    @FXML
    private void limpar(){
        txtNovoBrinco.clear();
        txtNovoNome.clear();
        txtNovaRaca.clear();
    }
    
    @FXML
    private void cancelar() throws IOException{
        App.setRoot("Menu");
    }
    
    private void mostrarAlerta(Alert.AlertType tipo, String titulo, String mensagem){
        Alert alert = new Alert(tipo);
        alert.setTitle(titulo);
        alert.setHeaderText(null);
        alert.setContentText(mensagem);
        alert.showAndWait();
    }
}
