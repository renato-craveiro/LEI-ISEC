package pt.isec.pa.javalife.fsmStudyGumball.model.fsm;

import pt.isec.pa.javalife.fsmStudyGumball.model.data.GumballMachineData;
import pt.isec.pa.javalife.fsmStudyGumball.model.fsm.states.MaintenanceState;
import pt.isec.pa.javalife.fsmStudyGumball.model.fsm.states.WithCoinState;
import pt.isec.pa.javalife.fsmStudyGumball.model.fsm.states.WithoutCoinState;

import javax.xml.stream.FactoryConfigurationError;

public interface IState {
    boolean insertCoin();
    boolean ejectCoin();
    boolean turnsCrank();
    boolean startMaintenance();
    boolean refillGumballs(int count);
    State getState();

    static IState getInstance(State state,
                              Context context, GumballMachineData data) {
        return switch (state) {
            case MAINTENANCE -> new MaintenanceState(context, data);
            case WITH_COIN -> new WithCoinState(context, data);
            case WITHOUT_COIN -> new WithoutCoinState(context, data);
        };
    }


}