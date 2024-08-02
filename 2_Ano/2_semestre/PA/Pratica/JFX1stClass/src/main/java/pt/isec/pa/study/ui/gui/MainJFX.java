package pt.isec.pa.study.ui.gui;

import javafx.application.Application;
import javafx.scene.Scene;
import javafx.stage.Stage;
import pt.isec.pa.study.ui.ModelData;
import pt.isec.pa.study.ui.gui.panes.RootPane;

public class MainJFX extends Application {
    ModelData data;
    public MainJFX() { data = new ModelData(); } // It can also be created in 'init'
    @Override
    public void start(Stage stage) throws Exception {
        //data = new ModelData();
        RootPane root = new RootPane(data);
        Scene scene = new Scene(root,600,400);
        stage.setScene(scene);
        stage.setTitle("JavaFX");
        stage.show();
    }
}