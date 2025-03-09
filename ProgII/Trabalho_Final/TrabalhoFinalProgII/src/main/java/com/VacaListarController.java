package com;

import java.io.IOException;
import java.util.List;
import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.fxml.FXML;
import javafx.scene.control.TableColumn;
import javafx.scene.control.TableView;
import javafx.scene.control.cell.PropertyValueFactory;
import modelo.Vaca;
import util.Dao;

public class VacaListarController {
    
    @FXML
    private TableView<Vaca> tabelaVacas;
    
    @FXML
    private TableColumn<Vaca, String> colBrinco;
    
    @FXML
    private TableColumn<Vaca, String> colNome;
    
    @FXML
    private TableColumn<Vaca, String> colRaca;
    
    private Dao<Vaca> daoVaca;
    
    @FXML
    private void cancelar() throws IOException{
        App.setRoot("Menu");
    }
    
    @FXML
    private void initialize(){
        daoVaca = new Dao<>(Vaca.class);
        
        colBrinco.setCellValueFactory(new PropertyValueFactory<>("brinco"));
        colNome.setCellValueFactory(new PropertyValueFactory<>("nome"));
        colRaca.setCellValueFactory(new PropertyValueFactory<>("raca"));

        carregarVacas();
    }
    
    private void carregarVacas(){
        List<Vaca> vacas = daoVaca.listarTodos();
        ObservableList<Vaca> observableVacas = FXCollections.observableArrayList(vacas);
        tabelaVacas.setItems(observableVacas);
    }
}
