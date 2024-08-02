package pt.isec.a2018011392;

import java.util.List;

public class RecentBook extends Book{
    String ISBN;
    double price;

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

    public RecentBook(String title, List<String> author) {
        super(title, author);
    }
}
