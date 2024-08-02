package pt.isec.a2018011392;

import javax.naming.Context;

public class ClosedState extends StateAdapter {
    protected ClosedState(Context context, SafeDate data) {
        super(context,data);
    }



    @Override
    public boolean openAndCorrectCode() {
        changeState(new OpenState(context, data));
        return true;
    }
    @Override
    public boolean openAndWrongCode() {
        data.incrementAttempts();
        if(data.attempts >= 3){
            changeState(new LockedState(context, data));
        }
        return true;
    }

    @Override
    public State getState() { return State.CLOSED; }
}
}

