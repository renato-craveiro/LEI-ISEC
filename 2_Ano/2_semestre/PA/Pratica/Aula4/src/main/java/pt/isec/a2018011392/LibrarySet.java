package pt.isec.a2018011392;

import java.util.HashSet;
import java.util.List;

public class LibrarySet implements ILibrary {
    HashSet<Book> books;
    String name;

    public LibrarySet(String name) {
        this.name = name;
        books = new HashSet<>();
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
    public int addBook(String title, List<String> authors, Book b) {
        Book book=b;
        books.add(book);
        return book.getId();
    }

    @Override
    public Book findBook(String title) throws CloneNotSupportedException {
        for(Book book:books) {
            if (book.getTitle().equals(title)) {
                return (Book) book.clone();
            }
        }
        return null;
    }

    @Override
    public Book findBook(int id) throws CloneNotSupportedException {
        for(Book book:books) {
            if (book.getId()==id) {
                return (Book) book.clone();
            }
        }
        return null;
    }

    @Override
    public boolean removeBook(String title) {
        for (Book book:books) {
            if (book.getTitle().equals(title)) {
                books.remove(book);
                return true;
            }
        }
        return false;
    }

    @Override
    public boolean removeBook(int id) {
        for (Book book:books) {
            if (book.getId()==id) {
                books.remove(book);
                return true;
            }
        }
        return false;
    }

    @Override
    public String toString() {
        return "Library{" +
                "name='" + name + '\'' +
                ", books=" + books +
                '}';
    }
}
