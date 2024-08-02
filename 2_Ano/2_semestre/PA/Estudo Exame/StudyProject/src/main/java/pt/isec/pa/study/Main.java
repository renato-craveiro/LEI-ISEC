package pt.isec.pa.study;

import pt.isec.pa.study.FSM.MainFSM;
import pt.isec.pa.study.Factory.MainFactory;
import pt.isec.pa.study.ui.utils.PAInput;

public class Main {
    public static void main(String[] args) {
        while (true){
            switch (PAInput.chooseOption("Choose the design pattern to run",
                    "FSM (smartphone on/off/lock/unlock)",
                    "Factory (burger restaurant)",
                    "Exit")) {
                case 1 -> MainFSM.main(args);
                case 2 -> MainFactory.main(args);
                case 3 -> {
                    return;
                }
                default -> System.out.println("Invalid option");
            }
        }
    }
}
