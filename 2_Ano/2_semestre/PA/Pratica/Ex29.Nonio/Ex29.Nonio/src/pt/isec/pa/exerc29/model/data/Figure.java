package pt.isec.pa.exerc29.model.data;

import javafx.scene.paint.Color;
import javafx.scene.shape.*;
import pt.isec.pa.exerc29.utils.AppLog;

//import java.awt.*;

public class Figure {
    private double x1,y1;
    private double x2,y2;
    private double r,g,b; //values 0..1
    private boolean fill;

    private boolean center;
    private double centerX, centerY;
    private double width;
    private double height;

    Shape shape;

    public Shape getShape() {
        return shape;
    }

    public void setShape(Shape shape) {



        this.shape = shape;
    }



    public Figure() {
        shape = new Line();
        x1 =  200   ; y1 =  200;
        x2 = 150; y2 = 200;
        r = 0;
        g = 0.75;
        b = 0.25;


        fill = false;
        AppLog.getInstance().add("new "+toString());
    }

    public void defineColor() {
        r = Math.random();
        g = Math.random();
        b = Math.random();
        shape.setStroke(new Color(r, g, b, 1).darker());
        if(isFill()) {
            shape.setFill(new Color(r, g, b, 1));

        }
        else {
            shape.setFill(null);
        }
            //shape.setFill(new Color(0,0,0,0));
        //System.out.println("Color defined: "+toString());
    }

    public void defineColor(double r, double g, double b) {
        this.r = r;
        this.g = g;
        this.b = b;

    }


    public Color getColor() {
        return new Color(r,g,b,1);
    }

    public double getX1() {
        return x1;
    }

    public void setX1(double x1) {
        this.x1 = x1;
    }

    public double getY1() {
        return y1;
    }

    public void setY1(double y1) {
        this.y1 = y1;
    }

    public double getX2() {
        return x2;
    }

    public void setX2(double x2) {
        this.x2 = x2;
    }

    public double getY2() {
        return y2;
    }

    public void setY2(double y2) {
        this.y2 = y2;
    }

    public double getR() {
        return r;
    }

    public void setR(double r) {
        this.r = r;
    }

    public double getG() {
        return g;
    }

    public void setG(double g) {
        this.g = g;
    }

    public double getB() {
        return b;
    }

    public void setB(double b) {
        this.b = b;
    }

    public boolean isFill() {
        return fill;
    }

    public void setFill(boolean fill) {
        this.fill = fill;
    }

    public boolean isCenter() {
        return center;
    }

    public double getCenterX() {
        return centerX;
    }

    public double getCenterY() {
        return centerY;
    }

    public void setCenter(double x, double y) {
        this.centerX = x;
        this.centerY = y;
        center = true;
    }

    public void centerOff() {
        center = false;
    }

    public void centerOn() {
        this.center = true;
    }

    public double getWidth() {
        return width;
    }

    public void setWidth(double width) {
        this.width = width;
    }

    public double getHeight() {
        return height;
    }

    public void setHeight(double height) {
        this.height = height;
    }



    /* TODO - Getters */

    /* TODO - Setters */

    @Override
    public String toString() {
        return String.format("Figure { (%.2f,%.2f)-(%.2f,%.2f) color= [%.2f,%.2f,%.2f] }",
                x1,y1,x2,y2,r,g,b);
    }
}
