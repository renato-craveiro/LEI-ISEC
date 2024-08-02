package pt.isec.pa.study.ui;

public class ModelData {

    String bgColour;
    int green, blue, oth;
    public ModelData() {
        bgColour = "black";
        green = blue = oth = 0;
    }

    public String getBgColour() {
        return bgColour;
    }

    public void setBgColour(String bgColour) {
        this.bgColour = bgColour;
    }

    public int getGreen() {
        return green;
    }

    public void incGreen(int green) {
        this.green = green+1;
    }

    public int getBlue() {
        return blue;
    }

    public void incBlue(int blue) {
        this.blue = blue+1;
    }

    public int getOth() {
        return oth;
    }

    public void incOth() {
        this.oth = oth+1;
    }
}
