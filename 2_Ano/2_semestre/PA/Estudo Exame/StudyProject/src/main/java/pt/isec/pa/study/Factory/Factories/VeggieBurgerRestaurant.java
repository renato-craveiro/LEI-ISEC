package pt.isec.pa.study.Factory.Factories;

import pt.isec.pa.study.Factory.data.Burger;
import pt.isec.pa.study.Factory.data.VeggieBurger;

public class VeggieBurgerRestaurant extends Restaurant{
    @Override
    public Burger createBurger() {
        return new VeggieBurger();
    }
}
