package pt.isec.pa.javalife.memento;

public interface IOriginator {
    IMemento save();
    void restore(IMemento memento);
}