package pt.isec.a2018011392;

import java.util.ArrayList;
import java.util.List;
import java.util.Objects;

public abstract class Book implements Cloneable {
    private static int all_id =0; //automatic
    private int id;
    private String title;
    private List<String> author;

    private int getNewId() {
        return all_id++;
    }


    public Book(String title, List<String> author) {
        this.id=getNewId();
        this.title = title;
        this.author = author;
    }

    public static int getAll_id() {
        return all_id;
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

    public List<String> getAuthor() {
        return author;
    }

    public void setAuthor(List<String> authors) {
        this.author = authors;
    }

    @Override
    public String toString() {
        return "pt.isec.a2018011392.Book{" +
                "id=" + id +
                ", title='" + title + '\'' +
                ", author='" + author + '\'' +
                '}';
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        //  if (o == null || getClass() != o.getClass()) return false;
        Book book = (Book) o;
        return Objects.equals(title, book.title);
    }

    @Override
    public int hashCode() {
        return Objects.hash(title);
    }


    @Override
    protected Object clone() throws CloneNotSupportedException {
        Book book = (Book) super.clone();
        book.setAuthor(List.copyOf(this.author));
        return book;

        //return new Book(this.title, List.copyOf(this.author));
    }
}
