package pt.isec.pa.javalife.propChng.gui;

import javafx.application.Application;
import javafx.scene.Scene;
import javafx.stage.Stage;
import pt.isec.pa.javalife.propChng.data.ModelManager;
import pt.isec.pa.javalife.propChng.gui.panes.RootPane;
import pt.isec.pa.javalife.propChng.gui.panes.RootPane2;

public class MainJFX extends Application {
    ModelManager mm;
    public MainJFX() {
        mm = new ModelManager();
    }
    @Override
    public void start(Stage stage) throws Exception {
        RootPane root = new RootPane(mm);
        Scene scene = new Scene(root,600,140);
        stage.setScene(scene);
        stage.setTitle("TeoPA@DEIS-ISEC");
        stage.show();
        Stage st2 = new Stage();
        RootPane2 root2 = new RootPane2(mm);
        Scene scene2 = new Scene(root2,150,50);
        st2.setScene(scene2);
        st2.setX(stage.getX()+stage.getWidth());
        st2.setY(stage.getY());
        st2.setTitle("Value");
        st2.show();
    }
}