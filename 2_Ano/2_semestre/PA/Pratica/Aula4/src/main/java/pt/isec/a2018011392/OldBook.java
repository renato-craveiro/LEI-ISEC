package pt.isec.a2018011392;

import java.util.List;

public class OldBook extends Book{
    int existingCopies;

    public int getExistingCopies() {
        return existingCopies;
    }

    public void setExistingCopies(int existingCopies) {
        this.existingCopies = existingCopies;
    }

    public OldBook(String title, List<String> author) {
        super(title, author);
    }
}
