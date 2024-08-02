package pt.isec.a2018011392;

abstract class SafeStateAdapter implements ISafeState{
    protected SafeContext context;
    protected SafeData data;

    protected SafeStateAdapter(SafeContext context, SafeData data) {
        this.context = context;
        this.data = data;
    }

    protected void changeState(ISafeState state) {
        context.changeState(state);
    }

    @Override
    public boolean isOpen() {
        return false;
    }

    @Override
    public boolean isLocked() {
        return false;
    }

    @Override
    public int getFailedAttempts() {
        return 0;
    }

    @Override
    public double getMoney() {
        return 0;
    }

    @Override
    public boolean open(String pin) {
        return false;
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
    public boolean unlock(String puk) {
        return false;
    }

    @Override
    public boolean depositMoney(double amount) {
        return false;
    }

    @Override
    public boolean withdrawMoney(double amount) {
        return false;
    }

    @Override
    public SafeState getState() {
        return null;
    }
}
