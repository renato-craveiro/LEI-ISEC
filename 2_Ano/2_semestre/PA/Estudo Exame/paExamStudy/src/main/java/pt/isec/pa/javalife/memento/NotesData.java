package pt.isec.pa.javalife.memento;

import java.util.ArrayList;
import java.util.HashMap;

public class NotesData implements Cloneable{

    String title;
    HashMap<String, ArrayList<String>> notesData;

    public NotesData(String title) {
        this.title = title;
        this.notesData = new HashMap<>();
    }

    public void addNote(String category, String note) {
        if (notesData.containsKey(category)) {
            notesData.get(category).add(note);
        } else {
            ArrayList<String> notes = new ArrayList<>();
            notes.add(note);
            notesData.put(category, notes);
        }
    }

    public void removeNote(String category, String note) {
        if (notesData.containsKey(category)) {
            notesData.get(category).remove(note);
        }
    }

    public String getNote(String category, int index) {
        if (notesData.containsKey(category)) {
            return notesData.get(category).get(index);
        }
        return null;
    }

    public ArrayList<String> getNotes(String category) {
        if (notesData.containsKey(category)) {
            return notesData.get(category);
        }
        return null;
    }

    public void setTitle(String title) {
        this.title = title;
    }

    public String getTitle() {
        return title;
    }

    public void clear() {
        notesData.clear();
    }

    public void clearCategory(String category) {
        notesData.remove(category);
    }

    @Override
    public String toString() {
        StringBuilder sb = new StringBuilder();
        sb.append("Title: ").append(title).append("\n");
        for (String category : notesData.keySet()) {
            sb.append("Category: ").append(category).append("\n");
            for (String note : notesData.get(category)) {
                sb.append("Note: ").append(note).append("\n");
            }
        }
        return sb.toString();
    }


    @Override
    public NotesData clone() {
        try {
            NotesData clone = (NotesData) super.clone();
            // TODO: copy mutable state here, so the clone can't change the internals of the original
            return clone;
        } catch (CloneNotSupportedException e) {
            throw new AssertionError();
        }
    }
}
