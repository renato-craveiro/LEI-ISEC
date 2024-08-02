package pt.isec.pa.study.FSM;

import pt.isec.pa.study.FSM.Phone;
import pt.isec.pa.study.ui.utils.PAInput;

// Press Shift twice to open the Search Everywhere dialog and type `show whitespaces`,
// then press Enter. You can now see whitespace characters in your code.
public class MainFSM {
    public static void main(String[] args) {
        Phone phone = new Phone();
        while (true) {
            switch (PAInput.chooseOption("Choose button", "Home", "Off/On", "Exit")) {
                case 1 -> System.out.println(phone.clickHome());
                case 2 -> System.out.println(phone.clickPower());
                case 3 -> {return;}
                default -> System.out.println("Invalid option");
            }
        }

    }
}