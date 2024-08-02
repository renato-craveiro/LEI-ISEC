package pt.isec.pa.javalife.fsmStudyGumball.model.fsm;

import pt.isec.pa.javalife.fsmStudyGumball.model.data.GumballMachineData;

public abstract class StateAdapter implements IState {
    protected Context context;
    protected GumballMachineData data;
    protected StateAdapter(Context context, GumballMachineData data) {
        this.context = context;
        this.data = data;
    }
    protected void changeState(IState newState) { //passa a State e em cada um dos estados, chama o changeState e passa o enum relativo

        context.changeState(newState); //-> original
       // State.getInstance(newState, context, data); // -> modified
        //context.changeState(State.getInstance(newState, context, data));
    }
    @Override
    public boolean insertCoin() { return false; }
    @Override
    public boolean ejectCoin() { return false; }
    @Override
    public boolean turnsCrank() { return false; }
    @Override
    public boolean startMaintenance() { return false; }
    @Override
    public boolean refillGumballs(int count) { return false; }

}
