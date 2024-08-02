package pt.isec.pa.javalife.propChng.data;

import java.beans.PropertyChangeListener;
import java.beans.PropertyChangeSupport;

public class ModelManager {
    public static final String PROP_VALUE = "prop_value";
    MyModel model;
    PropertyChangeSupport pcs;

    public ModelManager() {
        model = new MyModel(50);
        pcs = new PropertyChangeSupport(this);
    }

    public void addPropertyChangeListener(String property, PropertyChangeListener listener) {
        pcs.addPropertyChangeListener(property, listener);
    }

    public int getValue() {
        return model.getValue();
    }

    public void setValue(int value) {
        int old = model.getValue();
        model.setValue(value);
        pcs.firePropertyChange(PROP_VALUE, old, model.getValue());
    }

    public void inc() {
        model.inc();
        pcs.firePropertyChange(PROP_VALUE, null, null);
    }

    public void dec() {
        model.dec();
        pcs.firePropertyChange(PROP_VALUE, null, null);
    }
}

