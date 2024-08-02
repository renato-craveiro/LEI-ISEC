package pt.isec.pa.study.Factory;

import pt.isec.pa.study.Factory.Factories.HamBurgerRestaurant;
import pt.isec.pa.study.Factory.Factories.Restaurant;
import pt.isec.pa.study.Factory.Factories.VeggieBurgerRestaurant;
import pt.isec.pa.study.Factory.data.Burger;
import pt.isec.pa.study.ui.utils.PAInput;

public class MainFactory {
    public static void main(String[] args) {
        Restaurant restaurant;
        while (true){
            switch (PAInput.chooseOption("Choose the burger to order",
                    "Hamburger",
                    "Veggieburger",
                    "Exit")) {
                case 1 -> {
                    restaurant = new HamBurgerRestaurant();
                    Burger burger = restaurant.orderBurger();
                    burger.eat();
                }
                case 2 ->{
                    restaurant = new VeggieBurgerRestaurant();
                    Burger veggieBurger = restaurant.orderBurger();
                    veggieBurger.eat();
                }
                case 3 -> {
                    return;
                }
                default -> System.out.println("Invalid option");
            }
        }

    }
}
