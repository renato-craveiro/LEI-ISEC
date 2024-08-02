package pt.isec.pa.javalife.fsmStudyGumball.model.fsm.states;

import pt.isec.pa.javalife.fsmStudyGumball.model.data.GumballMachineData;
import pt.isec.pa.javalife.fsmStudyGumball.model.fsm.Context;
import pt.isec.pa.javalife.fsmStudyGumball.model.fsm.State;
import pt.isec.pa.javalife.fsmStudyGumball.model.fsm.StateAdapter;

public class WithCoinState extends StateAdapter {
    public WithCoinState(Context context, GumballMachineData data) {
        super(context,data);
    }
    @Override
    public boolean ejectCoin() {
        changeState(new WithoutCoinState(context, data));
        return true;
    }
    @Override
    public boolean turnsCrank() {
        if (data.getGumball() && data.getCount()>0) {
            changeState(new WithoutCoinState(context, data));
            return true;
        }
        changeState(new MaintenanceState(context,data));
        return false;
    }
    @Override
    public State getState() { return State.WITH_COIN; }

}
