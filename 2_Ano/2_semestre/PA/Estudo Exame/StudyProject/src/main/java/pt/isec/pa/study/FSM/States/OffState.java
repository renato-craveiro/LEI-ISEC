package pt.isec.pa.study.FSM.States;

import pt.isec.pa.study.FSM.Phone;

public class OffState extends State{
    public OffState(Phone phone) {
        super(phone);
    }

    @Override
    public String onHome() {
        phone.setState(new LockedState(phone));
        return phone.turnOn();
    }

    @Override
    public String onOffOn() {
        phone.setState(new LockedState(phone));
        return phone.turnOn();
    }

}
