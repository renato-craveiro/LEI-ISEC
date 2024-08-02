package pt.isec.a2018011392;

public class ClosedState extends SafeStateAdapter {
    public ClosedState(SafeContext context, SafeData data) {
        super(context, data);
    }

    @Override
    public boolean open(String pin) {
        if(data.open(pin)){
            changeState(new OpenState(context, data));
            return true;
        }

        System.out.println("Is locked? "+data.isLocked());
            //data.lock();
        if (data.isLocked()) {

            System.out.println("The safe is locked");
            changeState(new LockedState(context, data));

        }





        return false;
    }

    @Override
    public boolean lock() {
        if (data.lock()) {
            changeState(new LockedState(context, data));
            return true;
        }
        return false;
    }

    @Override
    public SafeState getState() {
        return SafeState.CLOSED;
    }
}
