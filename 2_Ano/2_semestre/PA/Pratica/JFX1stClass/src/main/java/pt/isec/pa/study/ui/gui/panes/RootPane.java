package pt.isec.pa.study.ui.gui.panes;

import javafx.geometry.Pos;
import javafx.scene.control.Button;
import javafx.scene.control.ColorPicker;
import javafx.scene.control.Label;
import javafx.scene.control.TextField;
import javafx.scene.layout.*;
import javafx.scene.paint.Color;
import pt.isec.pa.study.ui.ModelData;

import java.awt.*;

public class RootPane extends VBox { //View-Controller
    ModelData data;

    HBox hbox;
    Button b1,b2,bPersonalized;
    TextField tfPersonalized;

    Label gr,bl,personalized;

    // variables, including reference to views
    public RootPane(ModelData data) {
        this.data = data;
        createViews();
        registerHandlers();
        update();
    }
    private void createViews() {
        //setMaxSize(Double.MAX_VALUE, Double.MAX_VALUE);

        BorderPane bPane = new BorderPane();
        Thread.setDefaultUncaughtExceptionHandler((t, e) -> {
            data.setBgColour("black");
            update();
        });

        b1 = new Button("Green");
        b2 = new Button("Blue");
        bPersonalized = new Button("Set Color >");
        tfPersonalized = new TextField();
        hbox = new HBox();


        gr = new Label("Green: "+data.getGreen());
        bl = new Label("Blue: "+data.getBlue());
        personalized = new Label("Other: "+data.getOth());

        hbox.getChildren().addAll(b1,b2,tfPersonalized,bPersonalized);

        hbox.setPadding(new javafx.geometry.Insets(20));
        hbox.setSpacing(10);
        hbox.setMaxSize(Double.MAX_VALUE, Double.MAX_VALUE);

        HBox bottomHBox = new HBox();
        //bottomHBox.setMaxHeight(30);
        bottomHBox.setStyle("-fx-background-color: #585656;");
        bottomHBox.setPadding(new javafx.geometry.Insets(10));

        bottomHBox.setStyle("-fx-font-family: Consolas; -fx-font-size: 12; " +
                "-fx-text-fill: #585656; " +
                "-fx-background-color: gray;");
        //bottomHBox.setStyle("-fx-font-size: 20;");
        //bottomHBox.setMaxHeight(20);
        bottomHBox.setAlignment(javafx.geometry.Pos.CENTER);
        bottomHBox.getChildren().addAll(gr,bl,personalized);
        bottomHBox.setSpacing(10);

        HBox hb1 = new HBox();
        HBox hb2 = new HBox();
        HBox hb3 = new HBox();
        hb1.setMaxHeight(30);
        hb2.setMaxHeight(30);
        hb3.setMaxHeight(30);
        //hb1.getChildren().addAll(new Label("Green"),new Label("Blue"),new Label("Other"));
        bPane.setTop(hbox);
        bPane.setBottom(bottomHBox);

        getChildren().addAll(bPane);//hbox,hb1,hb2,hb3,bottomHBox);
        //hbox.setAlignment(Pos.TOP_CENTER);
        bottomHBox.setAlignment(Pos.BOTTOM_CENTER);


        //  setVgrow(this, Priority.ALWAYS);´~
        bPane.setPrefHeight(400);
        bPane.setPrefWidth(600);
        setPrefHeight(400);
        setPrefWidth(600);

        setMaxSize(Double.MAX_VALUE, Double.MAX_VALUE);
        bPane.setMaxSize(Double.MAX_VALUE, Double.MAX_VALUE);
        setVgrow(bPane, Priority.ALWAYS);
        //setAlignment(javafx.geometry.Pos.CENTER);




        /* create and configure views */ }
    private void registerHandlers() { /* handlers/listeners */
        b1.setOnAction(e -> {
            data.setBgColour("green");
            data.incGreen(data.getGreen());
            update();
        });
        b2.setOnAction(e -> {
            data.setBgColour("blue");
            data.incBlue(data.getBlue());
            update();
        });

        bPersonalized.setOnAction(e -> {
            data.incOth();

            if(tfPersonalized.getText().isEmpty())
                data.setBgColour("black");

            else if(Color.web(tfPersonalized.getText()) == null)
                data.setBgColour("black");

            else data.setBgColour(tfPersonalized.getText());

            update();
        });


        //bPersonalized.setOnAction(e -> data.setBgColour("yellow"));


    }
    private void update() { /* update views */

        setStyle("-fx-background-color: " + data.getBgColour() + ";");
        gr.setText("Green: "+data.getGreen());
        bl.setText("Blue: "+data.getBlue());
        personalized.setText("Other: "+data.getOth());
        //setStyle("-fx-background-color: " + data.getBgColour() + ";");
    }
}