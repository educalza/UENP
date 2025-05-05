package com;

import java.io.IOException;
import java.util.List;
import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.fxml.FXML;
import javafx.scene.control.Alert;
import javafx.scene.control.Alert.AlertType;
import javafx.scene.control.ComboBox;
import modelo.Usuario;
import util.Dao;

public class UsuarioExcluirController {
    
    @FXML
    private ComboBox<String> cbUsuarios;
    
    private Dao<Usuario> daoUsuario;
    private ObservableList<Usuario> listaUsuario;
    
    @FXML
    private void initialize(){
        daoUsuario = new Dao<>(Usuario.class);
        carregarUsuarios();
    }
    
    @FXML
    private void cancelar() throws IOException{
        App.setRoot("Menu");
    }
    
    @FXML
    private void excluir(){
        String loginExcluir = cbUsuarios.getValue();
        
        if(loginExcluir == null){
            mostrarAlerta(AlertType.WARNING, "AVISO", "Erro ao selecionar login");
            return;
        }
        
        Usuario usuario = daoUsuario.buscarPorChave("login", loginExcluir);
        
        if(usuario == null){
            mostrarAlerta(AlertType.WARNING, "AVISO", "O usuário não existe.");
            return;
        }
        
        boolean sucess = daoUsuario.excluir("login", loginExcluir);
        
        if(sucess){
            mostrarAlerta(AlertType.INFORMATION, "SUCESSO", "Usuário excluido com sucesso.");
            carregarUsuarios();
        } else{
            mostrarAlerta(AlertType.ERROR, "ERRO", "Erro desconhecido.");
        }
    }
    
    private void carregarUsuarios(){
        List<Usuario> usuarios = daoUsuario.listarTodos();
        ObservableList<String> logins = FXCollections.observableArrayList();
        for(Usuario usuario : usuarios){
            logins.add(usuario.getLogin());
        }
        cbUsuarios.setItems(logins);
    }
   
    private void mostrarAlerta(Alert.AlertType tipo, String titulo, String mensagem){
        Alert alert = new Alert(tipo);
        alert.setTitle(titulo);
        alert.setHeaderText(null);
        alert.setContentText(mensagem);
        alert.showAndWait();
    }
}
