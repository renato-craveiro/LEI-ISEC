package pt.isec.pa.exerc29.ui.gui;

import javafx.application.Application;
import javafx.scene.Scene;
import javafx.stage.Stage;
import pt.isec.pa.exerc29.model.ModelManager;
import pt.isec.pa.exerc29.ui.gui.log.LogStage;

public class MainJFX extends Application {
    ModelManager model;

    @Override
    public void init() throws Exception {
        super.init();
        model = new ModelManager();
    }

    @Override
    public void start(Stage stage) throws Exception {
        new LogStage(stage);

        RootPane rootPane= new RootPane(model);
        Scene scene = new Scene(rootPane,400,300);
        stage.setScene(scene);
        stage.setTitle("DEIS-ISEC");
        stage.show();
    }

}

