package pt.isec.pa.javalife.fsmStudyGumball.model.fsm;
import pt.isec.pa.javalife.fsmStudyGumball.model.data.GumballMachineData;
import pt.isec.pa.javalife.fsmStudyGumball.model.fsm.states.WithoutCoinState;

public class Context {
    private GumballMachineData data;
    private IState state;
    public Context(int count) {
        data = new GumballMachineData(count);
        state = new WithoutCoinState(this,data);
    }
    public State getState() {
        return state.getState();
    }
    void changeState(IState newState) {
        this.state = newState;
    }
    // ---- get data ----
    public int getCount() {
        return data.getCount();
    }

    // Transitions
    public boolean insertCoin() {
        return state.insertCoin();
    }
    public boolean ejectCoin() {
        return state.ejectCoin();
    }
    public boolean turnsCrank() {
        return state.turnsCrank();
    }
    public boolean startMaintenance() {
        return state.startMaintenance();
    }
    public boolean refillGumballs(int count) {
        return state.refillGumballs(count);
    }


}
