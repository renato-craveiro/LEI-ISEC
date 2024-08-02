package pt.isec.pa.study.FSM.States;

import pt.isec.pa.study.FSM.Phone;

public abstract class State {
    protected Phone phone;

    public State(Phone phone){
        this.phone = phone;
    }

    public abstract String onHome();
    public abstract String onOffOn();

}
