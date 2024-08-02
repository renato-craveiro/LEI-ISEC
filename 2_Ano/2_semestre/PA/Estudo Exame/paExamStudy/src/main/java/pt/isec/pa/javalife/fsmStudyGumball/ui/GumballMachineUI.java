package pt.isec.pa.javalife.fsmStudyGumball.ui;

import pt.isec.pa.javalife.fsmStudyGumball.model.fsm.Context;
import pt.isec.pa.javalife.utils.PAInput;

public class GumballMachineUI {
    private Context fsm;
    public GumballMachineUI(Context fsm) { this.fsm = fsm; }
    public boolean start() {
        while (switch (fsm.getState()) {
            case MAINTENANCE -> maintenance();
            case WITH_COIN -> withCoin();
            case WITHOUT_COIN -> withoutCoin();
        }) {
            System.out.printf("\nCurrent state: %s\n\n",fsm.getState()); // (only for debug)
        }
        return false;
    }
    public boolean withoutCoin() {
        System.out.printf("\nGumball Machine with %d gumballs\n", fsm.getCount());
        switch (PAInput.chooseOption("Machine without coin", "Insert coin", "Start maintenance", "Stop machine")) {
            case 1 -> fsm.insertCoin();
            case 2 -> fsm.startMaintenance();
            default -> {
                return false;
            }
        }
        return true;
    }

    public boolean withCoin() {
        System.out.printf("\nGumball Machine with %d gumballs\n",fsm.getCount());
        switch (PAInput.chooseOption("Machine with a coin","Eject coin","Turns crank","Stop Machine")) {
            case 1 -> fsm.ejectCoin();
            case 2 -> fsm.turnsCrank();
            default -> {
                return false;
            }
        }
        return true;
    }
    public boolean maintenance() {
        switch (PAInput.chooseOption("Maintenance/Machine without gumballs","Refill gumballs","Stop machine")) {
        case 1 -> {
            int count = PAInput.readInt("Number of Gumballs: ");
            fsm.refillGumballs(count);
        }
        default -> {
            return false;
        }
    }
        return true;
    }


}
