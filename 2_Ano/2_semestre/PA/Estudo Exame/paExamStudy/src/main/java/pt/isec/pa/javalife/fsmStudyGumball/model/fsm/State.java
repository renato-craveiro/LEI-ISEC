package pt.isec.pa.javalife.fsmStudyGumball.model.fsm;

import pt.isec.pa.javalife.fsmStudyGumball.model.data.GumballMachineData;
import pt.isec.pa.javalife.fsmStudyGumball.model.fsm.states.MaintenanceState;
import pt.isec.pa.javalife.fsmStudyGumball.model.fsm.states.WithCoinState;
import pt.isec.pa.javalife.fsmStudyGumball.model.fsm.states.WithoutCoinState;

public enum State {
    MAINTENANCE, WITH_COIN, WITHOUT_COIN;

    static IState getInstance(State state,
                              Context context, GumballMachineData data) {
        return switch (state) {
            case MAINTENANCE -> new MaintenanceState(context, data);
            case WITH_COIN -> new WithCoinState(context, data);
            case WITHOUT_COIN -> new WithoutCoinState(context, data);
        };
    }

}
