package pt.isec.pa.study.Factory.data;

public class HamBurger implements Burger{
    private String contents;
    @Override
    public void prepare() {
        System.out.println("Preparing HamBurger");
        try {
            Thread.sleep( 1000 );
        } catch (InterruptedException e) {
            throw new RuntimeException(e);
        }
        contents = "beef, cheese, lettuce, tomato, onion, pickles, ketchup, mustard, mayonnaise, bacon, mushrooms, and a fried egg.";
        System.out.println("HamBurger is ready");
    }

    @Override
    public void eat() {
        System.out.println("Eating HamBurger with " + contents);
    }
}
