package pt.isec.pa.study.FSM;

import pt.isec.pa.study.FSM.States.OffState;
import pt.isec.pa.study.FSM.States.State;

public class Phone {
    private State state;

    public Phone(){
        state = new OffState(this);
    }

    public void setState(State state){
        this.state = state;
    }

    public String lock(){
        return "Locking phone";
    }

    public String unlock(){
        return "Unlocking phone to home screen";
    }

    public String home(){
        return "Going to home screen";
    }

    public String turnOn(){
        return "Screen on, locked";
    }
     public String clickHome() {
        return state.onHome();
    }

    public String clickPower() {
        return state.onOffOn();
    }
}
