module com.meuteste {
    requires javafx.controls;
    requires javafx.fxml;

    opens com.meuteste to javafx.fxml;
    exports com.meuteste;
}
