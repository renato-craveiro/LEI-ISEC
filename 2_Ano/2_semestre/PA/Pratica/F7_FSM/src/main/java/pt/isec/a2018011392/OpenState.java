package pt.isec.a2018011392;

public class OpenState extends StateAdapter{
    OpenState(Context context, SafeDate data) {
        super(context,data);
    }

    @Override
    public boolean close() {
        changeState(new ClosedState(context, data));
        return true;
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
        return State.OPEN;
    }
}
