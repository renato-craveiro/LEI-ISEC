package pt.isec.pa.exerc29.model;

import pt.isec.pa.exerc29.model.data.Figure;
import javafx.scene.shape.Shape;

public class ModelManager {
    Figure figure;

    public ModelManager() {
        figure = new Figure();
    }

    public Figure getFigure() {
        return figure;
    }

    public void setShape(Shape shape) {
        figure.setShape(shape);
        if(figure.isFill())
            figure.defineColor();
    }

    /*public void getShape() {
        figure.getShape();
    }*/

    public void setXY1and2(double x1, double y1, double x2, double y2) {
        figure.setX1(x1);
        figure.setY1(y1);
        figure.setX2(x2);
        figure.setY2(y2);
    }




    public void defineColor() {
        figure.defineColor();
        //System.out.println("Color defined: " + figure.toString());
    }

    public void setFill(boolean fill) {
        figure.setFill(fill);
        //System.out.println("Fill defined: " + figure.toString());
    }

    /* TODO */
}
