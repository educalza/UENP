package com;

import java.io.IOException;
import java.util.List;
import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.fxml.FXML;
import javafx.scene.control.Alert;
import javafx.scene.control.ComboBox;
import javafx.scene.control.TextField;
import modelo.Usuario;
import util.Dao;

public class UsuarioEditarController {
    
    @FXML
    private ComboBox<String> cbUsuarios;
    
    @FXML private TextField txtNovoLogin;
    @FXML private TextField txtNovoNome;
    @FXML private TextField txtNovaSenha;
    @FXML private TextField txtNovaSenha2;
    
    private Dao<Usuario> daoUsuario;
    private Usuario usuario;
    private ObservableList<Usuario> listaUsuario;
    
    @FXML
    private void initialize(){
        daoUsuario = new Dao<>(Usuario.class);
        carregarUsuarios();
    }
    
    @FXML
    private void comboBox(){
        String loginEditar = cbUsuarios.getValue();
        
        if (loginEditar == null || loginEditar.isBlank()) {
            return;
        }
        
        usuario = daoUsuario.buscarPorChave("login", loginEditar.toUpperCase());
        if(usuario == null){
            mostrarAlerta(Alert.AlertType.WARNING, "AVISO", "O usuário não existe.");
            return;
        }
        
        txtNovoLogin.setText(usuario.getLogin());
        txtNovoNome.setText(usuario.getNome());
        txtNovaSenha.setText(usuario.getSenha());
        txtNovaSenha2.setText(usuario.getSenha());
    }
    
    @FXML
    private void confirmar(){
        if(usuario == null){
            mostrarAlerta(Alert.AlertType.WARNING, "AVISO", "Erro ao selecionar login");
            return;
        }
        
        String novoLogin = txtNovoLogin.getText();
        String novoNome = txtNovoNome.getText();
        String novaSenha = txtNovaSenha.getText();
        String novaSenha2 = txtNovaSenha2.getText();
        
        if(novoLogin.isBlank() || novoNome.isBlank() || novaSenha.isBlank() || novaSenha2.isBlank()){
            mostrarAlerta(Alert.AlertType.WARNING, "AVISO", "Preencha todos os campos");
            return;
        }
   
        if(novaSenha == null ? novaSenha2 != null : !novaSenha.equals(novaSenha2)){
            mostrarAlerta(Alert.AlertType.WARNING, "Aviso", "Senhas diferentes, digite a mesma senha.");
            return;
        }
        
        Usuario usuarioExistente = daoUsuario.buscarPorChave("login", novoLogin);
        if(usuarioExistente != null && !usuarioExistente.getLogin().equals(usuario.getLogin())){
            mostrarAlerta(Alert.AlertType.WARNING, "AVISO", "Login existente");
            return;
        }
        
        usuario.setLogin(novoLogin.toUpperCase());
        usuario.setNome(novoNome.toUpperCase());
        usuario.setSenha(novaSenha.toUpperCase());
        
        daoUsuario.alterar("login", cbUsuarios.getValue().toUpperCase(), usuario);
        mostrarAlerta(Alert.AlertType.INFORMATION, "AVISO", "Usuario alterado com sucesso");
        carregarUsuarios();
        limpar();
    }
    
    private void carregarUsuarios(){
        List<Usuario> usuarios = daoUsuario.listarTodos();
        ObservableList<String> logins = FXCollections.observableArrayList();
        for(Usuario usuario : usuarios){
            logins.add(usuario.getLogin());
        }
        cbUsuarios.setItems(logins);
    }
    
    @FXML
    private void limpar(){
        txtNovoLogin.clear();
        txtNovoNome.clear();
        txtNovaSenha.clear();
        txtNovaSenha2.clear();
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
