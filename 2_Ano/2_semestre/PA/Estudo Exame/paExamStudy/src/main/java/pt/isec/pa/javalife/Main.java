package pt.isec.pa.javalife;

import javafx.application.Application;
import pt.isec.pa.javalife.fsmStudyGumball.model.fsm.Context;
import pt.isec.pa.javalife.fsmStudyGumball.ui.GumballMachineUI;
import pt.isec.pa.javalife.propChng.gui.MainJFX;
import pt.isec.pa.javalife.singleton.ModelLog;
import pt.isec.pa.javalife.utils.PAInput;

import java.util.Date;

// Press Shift twice to open the Search Everywhere dialog and type `show whitespaces`,
// then press Enter. You can now see whitespace characters in your code.
public class Main {

    static boolean appLaunched = false;
    public static void main(String[] args) {

        while (true){
            switch (PAInput.chooseOption("Choose The program to run",
                    "Gumball Machine (FSM + factory)",
                    "Log (Singleton)",
                    "PropertyChange (Observer + JavaFX)",
                    "Exit")) {
                case 1 -> {
                    ModelLog.getInstance().log("Gumball Machine started ["+date()+"]");
                    gumball();
                }
                case 2 -> log();
                case 3 -> {
                    if(!appLaunched) {
                        appLaunched = true;
                        propChg(args);
                    } else {
                        System.out.println("Application already launched. Please exit the program to launch again.");
                    }
                }
                case 4 -> System.exit(0);
            }
        }
    }

    public static void gumball(){
        Context fsm = new Context(100);
        GumballMachineUI ui = new GumballMachineUI(fsm);
        ui.start();
    }

    public static void log(){
        switch (PAInput.chooseOption("Log", "Add log", "Show log", "Exit")) {
            case 1 -> ModelLog.getInstance().log("Custom log [" + date() + "]");
            case 2 -> ModelLog.getInstance().getLog().forEach(System.out::println);
            case 3 -> System.exit(0);
        }
    }

    public static void propChg(String[] args) {
        ModelLog.getInstance().log("Property Change started ["+date()+"]");
        Application.launch(MainJFX.class, args);
        ModelLog.getInstance().log("Property Change ended ["+date()+"]");
    }



    public static String date() {
        long currentTimeMillis = System.currentTimeMillis();
        Date date = new Date(currentTimeMillis);
        return "Current date and time: " + date;
    }



}