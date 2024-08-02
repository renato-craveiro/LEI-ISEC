package pt.isec.a2018011392;

import java.util.List;

public interface ILibrary {
    String getName();
    void setName(String name);
    int addBook(String title, List<String> authors,Book b);
    Book findBook(String title) throws CloneNotSupportedException;
    Book findBook(int id) throws CloneNotSupportedException;
    boolean removeBook(String title);
    boolean removeBook(int id);
}