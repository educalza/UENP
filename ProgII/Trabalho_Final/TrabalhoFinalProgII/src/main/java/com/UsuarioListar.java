package com;

import java.io.IOException;
import java.util.List;
import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.fxml.FXML;
import javafx.scene.control.TableColumn;
import javafx.scene.control.TableView;
import javafx.scene.control.cell.PropertyValueFactory;
import modelo.Usuario;
import util.Dao;

public class UsuarioListar {
    
    @FXML
    private TableView<Usuario> tabelaUsuarios;
    
    @FXML
    private TableColumn<Usuario, String> colLogin;
    
    @FXML
    private TableColumn<Usuario, String> colNome;
    
    @FXML
    private TableColumn<Usuario, String> colSenha;
    
    private Dao<Usuario> daoUsuario;
    
    @FXML
    private void cancelar() throws IOException{
        App.setRoot("Menu");
    }
    
    @FXML
    private void initialize(){
        daoUsuario = new Dao<>(Usuario.class);
        
        colLogin.setCellValueFactory(new PropertyValueFactory<>("login"));
        colNome.setCellValueFactory(new PropertyValueFactory<>("nome"));
        colSenha.setCellValueFactory(new PropertyValueFactory<>("senha"));

        carregarUsuarios();
    }
    
    private void carregarUsuarios(){
        List<Usuario> usuarios = daoUsuario.listarTodos();
        ObservableList<Usuario> observableUsuarios = FXCollections.observableArrayList(usuarios);
        tabelaUsuarios.setItems(observableUsuarios);
    }
}
