package pt.isec.pa.study.Factory.Factories;

import pt.isec.pa.study.Factory.data.Burger;
import pt.isec.pa.study.Factory.data.HamBurger;
import pt.isec.pa.study.Factory.data.VeggieBurger;

enum BurgerType{
    HAMBURGER, VEGGIEBURGER
}
public abstract class Restaurant {
    public Burger orderBurger (){
        Burger burger = createBurger();
        burger.prepare();
        return burger;
    }

    public abstract Burger createBurger();
}
