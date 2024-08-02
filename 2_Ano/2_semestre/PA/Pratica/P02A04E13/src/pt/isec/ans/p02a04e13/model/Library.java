package pt.isec.ans.p02a04e13.model;

import java.util.ArrayList;
import java.util.Collection;
import java.util.Collections;
import java.util.List;

public class Library implements ILibrary{

    private String name;
    private List<Book> books;

    public Library(String name) {
        this.name = name;
        this.books = new ArrayList<>();
    }

    @Override
    public String getName() {
        return name;
    }

    @Override
    public void setName(String name) {
        this.name = name;
    }

    @Override
    public int addOldBook(String title, List<String> authors, int nrCopies) {
        Book book = new OldBook(title,authors);
        if (books.contains(book))
            return -1;
        books.add(book);
        return book.getId();
    }

    @Override
    public int addRecentBook(String title, List<String> authors, String isbn, double cost) {
        Book book = new RecentBook(title,authors);
        if (books.contains(book))
            return -1;
        books.add(book);
        return book.getId();
    }

    @Override
    public Book findBook(String title) throws CloneNotSupportedException {
        for(Book book: books)
            if (book.getTitle().equals(title))
                return book.clone();
        return null;
    }


    @Override
    public Book findBook(int id) throws CloneNotSupportedException {
        for(Book book: books)
            if (book.getId() == id)
                return book.clone();
        return null;
    }

    @Override
    public boolean removeBook(String title) throws CloneNotSupportedException {
        for(Book book: books)
            if (book.getTitle().equals(title)) {
                books.remove(book.clone());
                return true;
            }
        return false;

    }

    @Override
    public boolean removeBook(int id) {
        for(Book book: books)
            if (book.getId() == id)
                return books.remove(book);
        return false;
    }

    @Override
    public String toStringSorted() {
        List<Book> sortedBooks = new ArrayList<>(books);
        Collections.sort(sortedBooks);
        StringBuilder output = new StringBuilder();
        output.append(String.format("Library %s:\n",name));
        for(Book book : sortedBooks)
            output.append(String.format("  - %s\n",book));
        return output.toString();
    }

    @Override
    public String toString() {
        StringBuilder output = new StringBuilder();
        output.append(String.format("Library %s:\n",name));
        for(Book book : books)
            output.append(String.format("  - %s\n",book));
        return output.toString();
    }
}
