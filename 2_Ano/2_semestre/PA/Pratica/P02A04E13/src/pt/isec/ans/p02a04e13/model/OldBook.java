package pt.isec.ans.p02a04e13.model;

import java.util.List;

public class OldBook extends Book{
    public int getExistingCopies() {
        return existingCopies;
    }

    public void setExistingCopies(int existingCopies) {
        this.existingCopies = existingCopies;
    }

    int existingCopies;
    public OldBook(String title, List<String> authors) {
        super(title, authors);
    }

    public OldBook(String title, String... authors) {
        super(title, authors);
    }
}
