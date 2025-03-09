package com;

import java.io.IOException;
import java.time.LocalDate;
import java.time.Month;
import java.util.List;
import java.util.stream.Collectors;
import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.fxml.FXML;
import javafx.scene.control.Alert;
import javafx.scene.control.ComboBox;
import javafx.scene.control.DatePicker;
import javafx.scene.control.TableColumn;
import javafx.scene.control.TableView;
import javafx.scene.control.cell.PropertyValueFactory;
import modelo.Producao;
import modelo.Vaca;
import util.Dao;

public class TotalProducaoLeiteController {
    
    @FXML
    private ComboBox<String> cbMes;
    
    @FXML
    private DatePicker dpDia;
    
    @FXML
    private TableView<Producao> tabelaVacas;
    
    @FXML
    private TableColumn<Producao, String> colBrinco;
    
    @FXML
    private TableColumn<Producao, String> colNome;
    
    @FXML
    private TableColumn<Producao, LocalDate> colData;
    
    @FXML
    private TableColumn<Producao, Double> colProducao;
    
    private Dao<Vaca> daoVaca;
    private Dao<Producao> daoProducao;
    private ObservableList<Producao> listaProducao;
    
    @FXML
    private void initialize() {
        System.out.println("Tela de Producao Total de Leite carregada!");
        daoVaca = new Dao<>(Vaca.class);
        daoProducao = new Dao<>(Producao.class);
        listaProducao = FXCollections.observableArrayList();

        // Configurar a tabela
        colBrinco.setCellValueFactory(new PropertyValueFactory<>("brinco"));
        colNome.setCellValueFactory(new PropertyValueFactory<>("nome"));
        colData.setCellValueFactory(new PropertyValueFactory<>("data"));
        colProducao.setCellValueFactory(new PropertyValueFactory<>("producao"));

        tabelaVacas.setItems(listaProducao);

        carregarMeses();
    }
    
    @FXML
    private void cancelar() throws IOException{
        App.setRoot("menu");
    }
    
    @FXML
    private void buscar() {
        LocalDate data = dpDia.getValue();
        String mes = cbMes.getValue();

        if (mes == null && data == null) {
            mostrarAlerta(Alert.AlertType.WARNING, "AVISO", "Selecione alguma data.");
            return;
        }

        List<Producao> producoes = daoProducao.listarTodos();

        if (data == null) { // Busca por mês
            if (mes == null) {
                mostrarAlerta(Alert.AlertType.WARNING, "AVISO", "Selecione um mês válido.");
                return;
            }

            try {
                Month mesSelecionado = Month.valueOf(mes.toUpperCase());
                List<Producao> producoesFiltradas = producoes.stream()
                        .filter(p -> p.getData() != null && p.getData().getMonth() == mesSelecionado)
                        .collect(Collectors.toList());

                atualizarTabela(producoesFiltradas);
            } catch (IllegalArgumentException e) {
                mostrarAlerta(Alert.AlertType.ERROR, "ERRO", "Mês inválido selecionado.");
            }
        } else { // Busca por dia
            List<Producao> producoesFiltradas = producoes.stream()
                    .filter(p -> p.getData() != null && p.getData().isEqual(data))
                    .collect(Collectors.toList());

            atualizarTabela(producoesFiltradas);
        }
    }
    
    private void carregarMeses() {
        ObservableList<String> meses = FXCollections.observableArrayList();
        for (Month mes : Month.values()) {
            meses.add(mes.name());
        }
        cbMes.setItems(meses);
    }
    
    private void atualizarTabela(List<Producao> producoes) {
        listaProducao.clear();
        for (Producao producao : producoes) {
            Vaca vaca = daoVaca.buscarPorChave("brinco", producao.getBrinco());
            if (vaca != null) {
                producao.setNome(vaca.getNome()); // Definir o nome para exibição
            }
            listaProducao.add(producao);
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
