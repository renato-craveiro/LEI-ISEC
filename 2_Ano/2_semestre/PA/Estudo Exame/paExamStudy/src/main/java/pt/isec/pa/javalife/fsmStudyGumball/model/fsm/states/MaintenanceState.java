package pt.isec.pa.javalife.fsmStudyGumball.model.fsm.states;

import pt.isec.pa.javalife.fsmStudyGumball.model.data.GumballMachineData;
import pt.isec.pa.javalife.fsmStudyGumball.model.fsm.Context;
import pt.isec.pa.javalife.fsmStudyGumball.model.fsm.State;
import pt.isec.pa.javalife.fsmStudyGumball.model.fsm.StateAdapter;

public class MaintenanceState extends StateAdapter {
    public MaintenanceState(Context context, GumballMachineData data) {
        super(context,data);
    }
    @Override
    public boolean refillGumballs(int count) {
        data.refillGumballs(count);
        if (data.getCount()>0) {
            changeState(new WithoutCoinState(context, data));
            return true;
        }
        return false;
    }
    @Override
    public State getState() {
        return State.MAINTENANCE;
    }

}
