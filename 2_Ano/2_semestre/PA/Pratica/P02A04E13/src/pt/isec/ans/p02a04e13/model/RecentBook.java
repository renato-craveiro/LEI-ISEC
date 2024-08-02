package pt.isec.ans.p02a04e13.model;

import java.util.List;

public class RecentBook extends Book{
    public String getISBN() {
        return ISBN;
    }

    public void setISBN(String ISBN) {
        this.ISBN = ISBN;
    }

    public double getPrice() {
        return price;
    }

    public void setPrice(double price) {
        this.price = price;
    }

    double price;
    String ISBN;
    public RecentBook(String title, List<String> authors) {
        super(title, authors);
    }

    public RecentBook(String title, String... authors) {
        super(title, authors);
    }
}
