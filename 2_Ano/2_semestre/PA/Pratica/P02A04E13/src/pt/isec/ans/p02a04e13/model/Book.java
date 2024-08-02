package pt.isec.ans.p02a04e13.model;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.Objects;

public abstract class Book implements Cloneable, Comparable<Book>{
    private static int counterId = 0;

    public static int getNewID() {
        return ++counterId;
    }

    private int id;
    private String title;
    private List<String> authors;

    public Book(String title, List<String> authors) {
        this.id = getNewID();
        this.title = title;
        this.authors = List.copyOf(authors);
    }

    public Book(String title, String... authors) {
        this(title,List.of(authors));
        /*this.id = getNewID();
        this.title = title;
        this.authors = List.of(authors);*/
        /*this.authors = new ArrayList<>();
        for(String author : authors)
            this.authors.add(author);*/
    }

    public int getId() {
        return id;
    }

    public String getTitle() {
        return title;
    }

    public void setTitle(String title) {
        this.title = title;
    }

    public List<String> getAuthors() {
        return List.copyOf(authors);
    }

    public void setAuthors(List<String> authors) {
        this.authors = List.copyOf(authors);
    }

    @Override
    public String toString() {
        String strAuthors = authors.toString();
        return String.format("[%d] %s - %s",id,title,
                        strAuthors.substring(1,strAuthors.length()-1));
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        //if (o == null || getClass() != o.getClass()) return false;

        Book book = (Book) o;

        return title.equals(book.title);
    }

    @Override
    public int hashCode() {
        return title != null ? title.hashCode() : 0;
    }

    @Override
    protected Book clone() throws CloneNotSupportedException {
        Book newBook = (Book) super.clone();
        //newBook.id = getNewID();
        newBook.authors = List.copyOf(authors);
        return newBook;
    }

    @Override
    public int compareTo(Book o) {
        return title.compareTo(o.title);
    }
}
