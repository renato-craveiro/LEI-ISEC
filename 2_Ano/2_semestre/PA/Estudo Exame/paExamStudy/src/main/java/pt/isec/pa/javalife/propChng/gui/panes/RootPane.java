package pt.isec.pa.javalife.propChng.gui.panes;

import javafx.geometry.Insets;
import javafx.geometry.Pos;
import javafx.scene.control.Button;
import javafx.scene.control.ProgressBar;
import javafx.scene.control.Slider;
import javafx.scene.layout.HBox;
import javafx.scene.layout.VBox;
import pt.isec.pa.javalife.propChng.data.ModelManager;

public class RootPane extends VBox {
    ModelManager mm;
    Slider slider;
    ProgressBar progressBar;
    Button btnPlus,btnMinus;
    public RootPane(ModelManager mm) {
        this.mm = mm;
        createViews();
        registerHandlers();
        update();
    }
    private void registerHandlers() {
        mm.addPropertyChangeListener(
                ModelManager.PROP_VALUE, evt -> { update(); }
        );
        btnMinus.setOnAction(actionEvent -> { mm.dec(); });
        btnPlus.setOnAction(actionEvent -> { mm.inc(); });
        slider.setOnMouseReleased(mouseEvent -> {
            mm.setValue((int)slider.getValue());
        });
        slider.setOnMouseDragged(mouseEvent -> {
            mm.setValue((int)slider.getValue());
        });
    }
    private void createViews() {
        setPadding(new Insets(10));
        setSpacing(10);
        slider = new Slider(0,100,50);
        slider.setShowTickMarks(true);
        slider.setShowTickLabels(true);
        slider.setMajorTickUnit(25);
        slider.setMinorTickCount(5);
        slider.setPrefWidth(Integer.MAX_VALUE);
        progressBar = new ProgressBar(0.5);
        progressBar.setPrefWidth(Integer.MAX_VALUE);
        HBox hbox = new HBox();
        btnMinus = new Button("-");
        btnMinus.setPrefWidth(Integer.MAX_VALUE);
        btnPlus = new Button("+");
        btnPlus.setPrefWidth(Integer.MAX_VALUE);
        hbox.setAlignment(Pos.CENTER);
        hbox.getChildren().addAll(btnMinus,btnPlus);
        this.getChildren().addAll(slider,progressBar,hbox);
    }
    private void update() {
        System.out.println(mm.getValue());
        slider.setValue(mm.getValue());
        progressBar.setProgress(mm.getValue()/100.0);
    }
}