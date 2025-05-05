package com;

import javafx.fxml.FXML;
import javafx.scene.control.Alert;
import javafx.scene.control.Alert.AlertType;
import javafx.scene.control.TextField;
import modelo.Usuario;
import util.Dao;

public class UsuarioCadastroController {
    
    //Cadastro Usuário
    @FXML private TextField txtUsuarioLogin;
    @FXML private TextField txtUsuarioNome;
    @FXML private TextField txtUsuarioSenha1;
    @FXML private TextField txtUsuarioSenha2;
    
    private Dao<Usuario> usuarioDao;

    @FXML
    private void initialize() {
        usuarioDao = new Dao<>(Usuario.class);
    }
    
    public void login() throws Exception{
        App.setRoot("UsuarioLogin");
    }
    
    public void voltar() throws Exception{
        App.setRoot("UsuarioLogin");
    }
    
    @FXML
    public void salvarUsuario(){
        String login = txtUsuarioLogin.getText().toUpperCase();
        String nome = txtUsuarioNome.getText();
        String senha1 = txtUsuarioSenha1.getText();
        String senha2 = txtUsuarioSenha2.getText();
        
        if(nome == null || login == null || senha1 == null || senha2 == null){
            mostrarAlerta(AlertType.WARNING, "Aviso", "Preencha todos os campos!");
            return;
        }
        
        if(senha1 == null ? senha2 != null : !senha1.equals(senha2)){
            mostrarAlerta(AlertType.WARNING, "Aviso", "Senhas diferentes, digite a mesma senha.");
            return;
        }
        
        try{
            Usuario usuarioExistente = usuarioDao.buscarPorChave("login", login);
            if(usuarioExistente != null){
                mostrarAlerta(AlertType.WARNING, "Aviso", "Usuário já existente");
                return;
            }
            
            Usuario usuario = new Usuario(login, nome, senha1);
            usuarioDao.inserir(usuario);
            limparCamposUsuario();
            
            mostrarAlerta(AlertType.INFORMATION, "Sucesso", "Usuário cadastrado.");
            
            login();
        } catch(Exception e){
            mostrarAlerta(AlertType.ERROR, "Erro", "Erro desconhecido "+e.getMessage());
        }
        
    }
    
    @FXML
    private void limparCamposUsuario(){
        txtUsuarioLogin.clear();
        txtUsuarioNome.clear();
        txtUsuarioSenha1.clear();
        txtUsuarioSenha2.clear();
    }

    private void mostrarAlerta(AlertType tipo, String titulo, String mensagem){
            Alert alert = new Alert(tipo);
            alert.setTitle(titulo);
            alert.setHeaderText(null);
            alert.setContentText(mensagem);
            alert.showAndWait();
    }
}
