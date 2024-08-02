package pt.isec.a2018011392;

public class LockedState extends SafeStateAdapter {
    public LockedState(SafeContext context, SafeData data) {
        super(context, data);
    }


    @Override
    public boolean unlock(String puk) {
        return data.unlock(puk);
    }

    @Override
    public SafeState getState() {
        return SafeState.LOCKED;
    }
}
