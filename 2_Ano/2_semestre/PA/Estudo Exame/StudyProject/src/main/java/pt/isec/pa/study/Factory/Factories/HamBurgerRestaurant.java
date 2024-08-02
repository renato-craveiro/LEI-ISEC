package pt.isec.pa.study.Factory.Factories;

import pt.isec.pa.study.Factory.data.Burger;
import pt.isec.pa.study.Factory.data.HamBurger;

public class HamBurgerRestaurant extends Restaurant{
    @Override
    public Burger createBurger() {
        return new HamBurger();
    }
}
