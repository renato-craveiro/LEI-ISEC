package pt.isec.pa.study.Factory.data;

import pt.isec.pa.study.Factory.data.Burger;

public class VeggieBurger implements Burger {
    String contents;
    @Override
    public void prepare() {
        System.out.println("Preparing VeggieBurger");
        try {
            Thread.sleep( 5000 );
        } catch (InterruptedException e) {
            throw new RuntimeException(e);
        }
        contents = "lettuce, tomato, onion, pickles, ketchup, mustard, mayonnaise, mushrooms.";
        System.out.println("VeggieBurger is ready");
    }

    @Override
    public void eat() {
        System.out.println("Eating VeggieBurger with "+contents);
    }
}
