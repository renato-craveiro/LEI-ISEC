package pt.isec.pa.javalife.propChng.data;

public class MyModel {
    private int value;
    public MyModel(int value) {
        this.value = value;
    }
    public int getValue() {
        return value;
    }
    public void setValue(int value) {
        this.value = Math.min(Math.max(0,value),100);
    }
    public void inc() {
        if (value < 100)
            value++;
    }
    public void dec() {
        if (value > 0)
            value--;
    }
}