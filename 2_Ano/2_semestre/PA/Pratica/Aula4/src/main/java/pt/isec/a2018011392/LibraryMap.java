package pt.isec.a2018011392;

import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;

public class LibraryMap implements ILibrary {
    HashMap<Integer,Book> books;
    String name;

    public LibraryMap(String name) {
        this.name = name;
        books = new HashMap<Integer,Book>();
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
    public int addBook(String title, List<String> authors,Book b) {
        Book book=b;
        books.put(book.getId(), book);
        return book.getId();
    }

    @Override
    public Book findBook(String title) throws CloneNotSupportedException {
        for (Map.Entry<Integer, Book> entry : books.entrySet()) {
            if (entry.getValue().getTitle().equals(title)) {
                return (Book) books.get(entry.getKey()).clone();
            }
        }
        return null;
    }

    @Override
    public Book findBook(int id) throws CloneNotSupportedException {
        return (Book) books.get(id).clone();
    }

    @Override
    public boolean removeBook(String title) {
        for (Map.Entry<Integer, Book> entry : books.entrySet()) {
            if (entry.getValue().getTitle().equals(title)) {
                books.remove(entry.getKey());
                return true;
            }
        }
        return false;
    }

    @Override
    public boolean removeBook(int id) {
        return books.remove(id) != null;
    }

    @Override
    public String toString() {
        return "Library{" +
                "name='" + name + '\'' +
                ", books=" + books +
                '}';
    }
}
