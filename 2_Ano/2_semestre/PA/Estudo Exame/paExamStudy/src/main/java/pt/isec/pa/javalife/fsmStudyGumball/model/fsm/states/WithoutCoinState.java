package pt.isec.pa.javalife.fsmStudyGumball.model.fsm.states;

import pt.isec.pa.javalife.fsmStudyGumball.model.data.GumballMachineData;
import pt.isec.pa.javalife.fsmStudyGumball.model.fsm.Context;
import pt.isec.pa.javalife.fsmStudyGumball.model.fsm.State;
import pt.isec.pa.javalife.fsmStudyGumball.model.fsm.StateAdapter;

public class WithoutCoinState extends StateAdapter {
    public WithoutCoinState(Context context, GumballMachineData data) {
        super(context,data);
    }
    @Override
    public boolean insertCoin() {
        changeState(new WithCoinState(context,data));
        return true;
    }
    @Override
    public boolean startMaintenance() {
        changeState(new MaintenanceState(context,data));
        return true;
    }
    @Override
    public State getState() { return State.WITHOUT_COIN; }

}
