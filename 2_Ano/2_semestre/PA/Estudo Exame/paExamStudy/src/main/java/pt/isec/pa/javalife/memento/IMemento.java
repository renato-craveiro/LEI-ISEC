package pt.isec.pa.javalife.memento;

public interface IMemento {
    default Object getSnapshot() { return null; }
}
