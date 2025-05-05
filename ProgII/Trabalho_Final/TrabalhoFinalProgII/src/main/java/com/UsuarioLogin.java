package com;

import java.io.IOException;
import javafx.fxml.FXML;
import javafx.scene.control.Alert;
import javafx.scene.control.Alert.AlertType;
import javafx.scene.control.TextField;
import modelo.Usuario;
import util.Dao;

public class UsuarioLogin {
    
    @FXML
    private TextField txtLogin;
    @FXML
    private TextField txtSenha;
    
    private Dao<Usuario> dao;
    
    @FXML
    private void initialize(){
        dao = new Dao<>(Usuario.class);
    }
    
    @FXML
    private void cadastrar() throws IOException{
        App.setRoot("UsuarioCadastro");
    }
    
    public void menu() throws Exception{
        App.setRoot("Menu");
    }
    
    @FXML
    private void entrar() throws Exception{
        if(txtLogin.getText().isBlank() || txtSenha.getText().isBlank()){
            mostrarAlerta(AlertType.WARNING, "Aviso", "Preencha todos os campos");
            return;
        }
        
        String login = txtLogin.getText().toUpperCase();
        Usuario usuario = dao.buscarPorChave("login", login);
        
        if(usuario == null){
            System.out.println("Usuario inexistente");
            mostrarAlerta(AlertType.WARNING, "AVISO", "Usuario inexistênte");
            return;
        }
        
        if(usuario.getSenha() == null || !usuario.getSenha().equals(txtSenha.getText())){
            System.out.println("Usuário encontrado: " + usuario.getLogin());
            mostrarAlerta(AlertType.WARNING, "AVISO", "Senha incorreta");
            return;
        }
        
        try{
            menu();
        } catch(IOException e){
            mostrarAlerta(AlertType.ERROR, "ERRO", "erro desconhecido " + e);
        }
    }
    
    
        private void mostrarAlerta(AlertType tipo, String titulo, String mensagem){
            Alert alert = new Alert(tipo);
            alert.setTitle(titulo);
            alert.setHeaderText(null);
            alert.setContentText(mensagem);
            alert.showAndWait();
    }
}
