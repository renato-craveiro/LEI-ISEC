package pt.isec.pa.exerc29.ui.gui;

import javafx.scene.control.ToggleButton;
import javafx.scene.control.ToggleGroup;
import javafx.scene.layout.BorderPane;
import javafx.scene.layout.HBox;
import javafx.scene.layout.Pane;
import javafx.scene.paint.Color;
import javafx.scene.shape.Ellipse;
import javafx.scene.shape.Line;
import javafx.scene.shape.Rectangle;
import pt.isec.pa.exerc29.model.ModelManager;
import pt.isec.pa.exerc29.utils.AppLog;

import java.awt.*;

public class RootPane extends BorderPane {
    private static final String LINE = "LINE";
    private static final String RECT = "RECT";
    private static final String ELLIPSE = "ELLIPSE";

    ModelManager model;
    Pane centralPane;
    ToggleGroup tgr;
    ToggleButton tbtOpt1, tbtOpt2, tbtOpt3;

    public RootPane(ModelManager model) {
        this.model = model;

        createViews();
        registerHandlers();
        update();
    }

    private void createViews() {
        HBox hbox = new HBox();
        tgr = new ToggleGroup();
        tbtOpt1 = new ToggleButton(LINE);
        tbtOpt1.setToggleGroup(tgr);
        tbtOpt1.setPrefWidth(Integer.MAX_VALUE);
        tbtOpt2 = new ToggleButton(RECT);
        tbtOpt2.setToggleGroup(tgr);
        tbtOpt2.setPrefWidth(Integer.MAX_VALUE);
        tbtOpt3 = new ToggleButton(ELLIPSE);
        tbtOpt3.setToggleGroup(tgr);
        tbtOpt3.setPrefWidth(Integer.MAX_VALUE);
        tgr.selectToggle(tbtOpt1);
        hbox.getChildren().addAll(tbtOpt1, tbtOpt2, tbtOpt3);
        this.setTop(hbox);
        centralPane = new Pane();
        setCenter(centralPane);
    }

    private void registerHandlers() {
        tbtOpt1.setOnAction(evt -> {update();
            AppLog.getInstance().add("Button LINE pressed");}
        );
        tbtOpt2.setOnAction(evt -> {update();
            AppLog.getInstance().add("Button RECT pressed");}
        );
        tbtOpt3.setOnAction(evt -> {update();
            AppLog.getInstance().add("Button ELLIPSE pressed");}
        );
        centralPane.setOnMouseClicked(evt -> {
            //System.out.println("lol");
            AppLog.getInstance().add(
                    String.format("x:%.2f y:%.2f Shift:%b Ctrl:%b Alt:%b",
                            evt.getX(),evt.getY(),evt.isShiftDown(), evt.isControlDown(),evt.isAltDown()
                    )
            );
            //System.out.println(AppLog.getInstance().getLog());
        });

        centralPane.setOnMousePressed(evt -> {
            model.getFigure().setX1(evt.getX());
            model.getFigure().setY1(evt.getY());
            model.setXY1and2(evt.getX(),evt.getY(),model.getFigure().getX2(),model.getFigure().getY2());

            AppLog.getInstance().add("MOUSE PRESSED");
            //System.out.println("x1 = "+model.getFigure().getX1()+" y1 = "+model.getFigure().getY1());
            update();
        });

        centralPane.setOnMouseDragged(evt -> {
            //model.getFigure().setX2(evt.getX());
            //model.getFigure().setY2(evt.getY());
            model.setXY1and2(model.getFigure().getX1(),model.getFigure().getY1(),evt.getX(),evt.getY());
            AppLog.getInstance().add("MOUSE DRAGGED ");
            //System.out.println("x2 = "+model.getFigure().getX2()+" y2 = "+model.getFigure().getY2());
            update();

            if(model.getFigure().isCenter())
                model.getFigure().setCenter(evt.getX(),evt.getY());

            //draw rectangle
            //Rectangle rect;
            if(model.getFigure().getX1() < model.getFigure().getX2()) {

                 //rect = new Rectangle(model.getFigure().getX1(), model.getFigure().getY1(), model.getFigure().getX2() - model.getFigure().getX1(), model.getFigure().getY2() - model.getFigure().getY1());
            } else {
                 //rect = new Rectangle(model.getFigure().getX2(), model.getFigure().getY2(), model.getFigure().getX1() - model.getFigure().getX2(), model.getFigure().getY1() - model.getFigure().getY2());
            }
                //rect.setStroke(model.getFigure().getColor().darker());
            //if(model.getFigure().isFill())
                //rect.setFill(model.getFigure().getColor());
            //centralPane.getChildren().add(rect);
        });

        centralPane.setOnMouseReleased(evt -> {
            AppLog.getInstance().add("MOUSE RELEASED");
            update();
        });

        setOnMouseMoved(evt -> {
            if(model.getFigure().isCenter()){
                model.getFigure().setX1(evt.getX());
                model.getFigure().setY1(evt.getY());
                //update();
            }
        });

        setOnKeyPressed(evt -> {
            //AppLog.getInstance().add("KEY PRESSED");
            if(evt.getCode()==javafx.scene.input.KeyCode.ALT)
                model.setFill(true);
            if(evt.getCode()==javafx.scene.input.KeyCode.CONTROL)
                model.getFigure().centerOn();

            //System.out.println("key pressed");
        });

        setOnKeyReleased(evt -> {
            //AppLog.getInstance().add("KEY RELEASED");
            if(evt.getCode()==javafx.scene.input.KeyCode.ALT)
                model.setFill(false);

            if(evt.getCode()==javafx.scene.input.KeyCode.CONTROL) {
                model.getFigure().setCenter(model.getFigure().getX1(), model.getFigure().getY1());
                model.getFigure().centerOff();
            }
            //System.out.println("key released");
        });



    }

    private void update() {
        //model.defineColor();
        centralPane.getChildren().clear();
        /*var line = new Line(50,50,200,100);
        var rect = new Rectangle(250,50,100,50);
        var elip = new Ellipse(225,175,75,50);
        /*line.setStroke(Color.INDIGO);
        rect.setStroke(model.getFigure().getColor().darker());
        elip.setStroke(model.getFigure().getColor().darker());*/


        model.defineColor();
        if (tbtOpt1.isSelected()) {
            model.setShape(new Line(model.getFigure().getX1(),model.getFigure().getY1(),model.getFigure().getX2(),model.getFigure().getY2()));
        } else if (tbtOpt2.isSelected()) {
            model.setShape(new Rectangle(model.getFigure().getX1(),model.getFigure().getY1(),model.getFigure().getX2(),model.getFigure().getY2()));
        } else if (tbtOpt3.isSelected()) {
            model.setShape(new Ellipse(model.getFigure().getX1(),model.getFigure().getY1(),model.getFigure().getX2(),model.getFigure().getY2()));
        }


        centralPane.getChildren().add(model.getFigure().getShape());


        /*if (tbtOpt1.isSelected()) {
            model.getFigure().setShape(new Line());
            model.setFill(false);
            //rect.setFill(null);
            //elip.setFill(null);
            line = new Line(model.getFigure().getX1(),model.getFigure().getY1(),model.getFigure().getX2(),model.getFigure().getY2());
            line.setStroke(model.getFigure().getColor().darker());
            if(model.getFigure().isFill())
                line.setFill(model.getFigure().getColor());
            centralPane.getChildren().add(line);

        } else if (tbtOpt2.isSelected()) {
            model.getFigure().setShape(new Rectangle());
            model.setFill(true);
            //rect.setFill(Color.ORANGE);
            //elip.setFill(Color.ORANGE);
            rect = new Rectangle(model.getFigure().getX1(),model.getFigure().getY1(),model.getFigure().getX2(),model.getFigure().getY2());
            rect.setStroke(model.getFigure().getColor().darker());
            if(model.getFigure().isFill())
                rect.setFill(model.getFigure().getColor());
            centralPane.getChildren().add(rect);
        } else if (tbtOpt3.isSelected()) {
            model.setFill(true);
            //rect.setFill(Color.color(Math.random(), Math.random(), Math.random()));
            //elip.setFill(Color.color(Math.random(), Math.random(), Math.random()));
            elip = new Ellipse(model.getFigure().getX1(),model.getFigure().getY1(),model.getFigure().getX2(),model.getFigure().getY2());
            elip.setStroke(model.getFigure().getColor().darker());
            if(model.getFigure().isFill()) {
                elip.setFill(model.getFigure().getColor());
                //Sys tem.out.println("fill = "+model.getFigure().getColor().toString());
            }
            centralPane.getChildren().add(elip);
        }*/
        //centralPane.getChildren().addAll(line,rect,elip);
    }
}
