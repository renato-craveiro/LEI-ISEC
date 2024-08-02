package pt.isec.pa.javalife.propChng.gui.panes;

import javafx.geometry.Pos;
import javafx.scene.control.TextField;
import javafx.scene.input.KeyCode;
import pt.isec.pa.javalife.propChng.data.ModelManager;

public class RootPane2 extends TextField {
    ModelManager mm;
    public RootPane2(ModelManager mm) {
        this.mm = mm;
        createViews();
        registerHandlers();
        update();
    }
    private void createViews() { this.setAlignment(Pos.CENTER); }
    private void registerHandlers() {
        mm.addPropertyChangeListener(ModelManager.PROP_VALUE, evt -> { update(); });
        this.setOnKeyPressed(keyEvent -> {
            if (keyEvent.getCode() == KeyCode.ENTER) {
                try {
                    mm.setValue(Integer.parseInt(this.getText()));
                } catch (Exception e) { }
            }
        });
    }

    private void update() { this.setText(""+mm.getValue()); }
}
