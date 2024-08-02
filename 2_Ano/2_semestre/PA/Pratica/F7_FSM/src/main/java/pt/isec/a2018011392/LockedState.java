package pt.isec.a2018011392;

import javax.naming.Context;

public class LockedState extends StateAdapter{
    LockedState(Context context, SafeDate data) {
        super(context,data);
    }

    @Override
    public boolean close() {
        return false;
    }

    @Override
    public boolean lock() {
        return false;
    }

    @Override
    public boolean openAndCorrectCode() {
        return false;
    }

    @Override
    public boolean openAndWrongCode() {
        return false;
    }

    @Override
    public State getState() {
        return State.LOCKED;
    }
}
