package pt.isec.ans.p02a04e13;

import pt.isec.ans.p02a04e13.model.Book;
import pt.isec.ans.p02a04e13.model.ILibrary;
import pt.isec.ans.p02a04e13.model.Library;
import pt.isec.ans.p02a04e13.ui.LibraryUI;

import java.util.List;

public class Main {
    public static void main(String[] args) throws CloneNotSupportedException {
        ILibrary library = new Library("DEIS-ISEC-List");
        LibraryUI ui = new LibraryUI(library);
        ui.start();

    }
}
