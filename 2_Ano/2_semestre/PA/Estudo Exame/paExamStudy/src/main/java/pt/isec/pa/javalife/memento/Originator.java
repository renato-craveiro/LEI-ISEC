package pt.isec.pa.javalife.memento;

import java.io.Serializable;

public class Originator implements IOriginator/*, Serializable */{

    NotesData notesData;

    private static class Memento implements IMemento {
        NotesData data;
        Memento(Originator base) {
            this.data = base.notesData.clone();
        }
    }


    @Override
    public IMemento save() {
        return new Memento(this);
    }

    @Override
    public void restore(IMemento memento) {
        //Object obj = memento.getSnapshot();
        if(memento instanceof Memento m){
            notesData = m.data;
        }
    }
}
