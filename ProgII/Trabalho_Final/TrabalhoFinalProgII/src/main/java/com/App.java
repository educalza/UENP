package com;

import javafx.application.Application;
import javafx.fxml.FXMLLoader;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.stage.Stage;

import java.io.IOException;

/**
 * JavaFX App
 */
public class App extends Application {

    private static Scene scene;
    private static Stage stage; // Adicionando referência ao Stage

    @Override
    public void start(Stage primaryStage) throws IOException {
        stage = primaryStage; // Armazena o Stage para uso global
        scene = new Scene(loadFXML("UsuarioLogin"));
        stage.setScene(scene);
        stage.show();
    }

    public static void setRoot(String fxml) throws IOException {
        scene.setRoot(loadFXML(fxml));
        stage.sizeToScene(); // Ajusta o tamanho da tela conforme o novo FXML
    }

    private static Parent loadFXML(String fxml) throws IOException {
        FXMLLoader fxmlLoader = new FXMLLoader(App.class.getResource(fxml + ".fxml"));
        return fxmlLoader.load();
    }

    public static Stage getStage() {
        return stage;
    }

    public static void main(String[] args) {
        launch();
    }
}
