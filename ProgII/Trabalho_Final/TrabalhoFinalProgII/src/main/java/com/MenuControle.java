package com;

import java.io.IOException;
import javafx.fxml.FXML;
import javafx.scene.control.Alert;
import javafx.scene.control.Button;
import javafx.scene.control.MenuItem;
import javafx.scene.control.SplitMenuButton;

public class MenuControle {

    @FXML
    private SplitMenuButton menuUsuarios;

    @FXML
    private SplitMenuButton menuVacas;

    @FXML
    private SplitMenuButton menuProducao;

    @FXML
    private Button btnSair;

    @FXML
    private void initialize() {
        configurarMenuUsuarios();
        configurarMenuVacas();
        configurarMenuProducao();
        configurarBotaoSair();
    }

    private void configurarMenuUsuarios() {
        if (menuUsuarios != null) {
            for (MenuItem item : menuUsuarios.getItems()) {
                switch (item.getText()) {
                    case "Listar":
                        item.setOnAction(e -> carregarTela("UsuarioConsultar"));
                        break;
                    case "Alterar":
                        item.setOnAction(e -> carregarTela("UsuarioEditar"));
                        break;
                    case "Excluir":
                        item.setOnAction(e -> carregarTela("UsuarioExcluir"));
                        break;
                }
            }
        } else {
            mostrarAlerta(Alert.AlertType.ERROR, "Erro", "Erro desconhecido");
        }
    }

    private void configurarMenuVacas() {
        if (menuVacas != null) {
            for (MenuItem item : menuVacas.getItems()) {
                switch (item.getText()) {
                    case "Cadastrar":
                        item.setOnAction(e -> carregarTela("VacaCadastro"));
                        break;
                    case "Listar":
                        item.setOnAction(e -> carregarTela("VacaConsultar"));
                        break;
                    case "Alterar":
                        item.setOnAction(e -> carregarTela("VacaEditar"));
                        break;
                    case "Excluir":
                        item.setOnAction(e -> carregarTela("VacaExcluir"));
                        break;
                }
            }
        } else {
            mostrarAlerta(Alert.AlertType.ERROR, "Erro", "Erro desconhecido");
        }
    }

    private void configurarMenuProducao() {
        if (menuProducao != null) {
            for (MenuItem item : menuProducao.getItems()) {
                switch (item.getText()) {
                    case "Cadastrar produção":
                        item.setOnAction(e -> carregarTela("ControleProducaoLeite"));
                        break;
                    case "Exibir produção por vaca":
                        item.setOnAction(e -> carregarTela("ProducaoPorVaca"));
                        break;
                    case "Exibir produção total da fazenda":
                        item.setOnAction(e -> carregarTela("ProducaoTotal"));
                        break;
                }
            }
        } else {
            mostrarAlerta(Alert.AlertType.ERROR, "Erro", "Erro desconhecido");
        }
    }

    private void configurarBotaoSair() {
        if (btnSair != null) {
            btnSair.setOnAction(e -> sair());
        } else {
            mostrarAlerta(Alert.AlertType.ERROR, "Erro", "Erro desconhecido");
        }
    }

    private void carregarTela(String fxml) {
        try {
            App.setRoot(fxml);
        } catch(IOException e){
            mostrarAlerta(Alert.AlertType.ERROR, "Erro", "Erro ao carregar a tela " + fxml);
        }
    }

    private void sair() {
        try {
            App.setRoot("UsuarioLogin");
        } catch(Exception e){
            mostrarAlerta(Alert.AlertType.ERROR, "Erro", "Erro desconhecido "+e.getMessage());
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
