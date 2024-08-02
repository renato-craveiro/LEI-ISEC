package pt.isec.a2018011392.ui;

import pt.isec.a2018011392.ISafeState;
import pt.isec.a2018011392.SafeContext;

import java.net.SocketOption;
import java.util.Scanner;

import static pt.isec.a2018011392.ui.PAInput.*;

public class SafeUI {
    private final SafeContext context;

    public SafeUI(SafeContext context) {
        this.context = context;
    }

    public void start() {

        while (true){
            switch (context.getState()) {
                case OPEN -> System.out.println("Safe is open");
                case CLOSED -> System.out.println("Safe is closed");
                case LOCKED -> System.out.println("Safe is locked");

            }
            Scanner sc = new Scanner(System.in);
            int a = sc.nextInt();

                    /*chooseOption("Choose an option:\n",
                    "1 - Open safe\n" +
                            "2 - Close safe\n" +
                            "3 - Lock safe\n" +
                            "4 - Unlock safe\n" +
                            "5 - Deposit money\n" +
                            "6 - Withdraw money\n" +
                            "7 - Get state\n" +
                            "0 - Exit\n");*/
            System.out.println("OPTUION:  "+a);
            switch (a) {
                case 1 -> open(readString("Enter pin: ",true));
                case 2 -> close();
                case 3 -> lock();
                case 4 -> unlock(readString("Enter puk: ",true));
                case 5 -> depositMoney(readNumber("Enter amount: "));
                case 6 -> withdrawMoney(readNumber("Enter amount: "));
                case 7 -> getState();
                case 0 -> {
                    System.out.println("Exiting...");
                    return;
                }
            }
        }

    }

    public void open(String pin) {
        if (context.open(pin)) {
            System.out.println("Safe is open");
        } else {
            System.out.println("Failed to open safe");
        }
    }

    public void close() {
        if (context.close()) {
            System.out.println("Safe is closed");
        } else {
            System.out.println("Failed to close safe");
        }
    }

    public void lock() {
        if (context.isLocked()) {
            System.out.println("Safe is locked");
        } else {
            System.out.println("Failed to lock safe");
        }
    }

    public void unlock(String puk) {
        if (context.unlock(puk)) {
            System.out.println("Safe is unlocked");
        } else {
            System.out.println("Failed to unlock safe");
        }
    }

    public void depositMoney(double amount) {
        if (context.depositMoney(amount)) {
            System.out.println("Deposited " + amount + "€");
        } else {
            System.out.println("Failed to deposit " + amount + "€");
        }
    }

    public void withdrawMoney(double amount) {
        if (context.withdrawMoney(amount)) {
            System.out.println("Withdrew " + amount + "€");
        } else {
            System.out.println("Failed to withdraw " + amount + "€");
        }
    }

    public void getState() {
        //ISafeState state = context.getState();
        System.out.println("State: " + context.getState());
        System.out.println("Is open: " + context.isOpen());
        System.out.println("Is locked: " + context.isLocked());
        System.out.println("Failed attempts: " + context.getFailedAttempts());
        System.out.println("Money: " + context.getMoney() + "€");
    }
}
