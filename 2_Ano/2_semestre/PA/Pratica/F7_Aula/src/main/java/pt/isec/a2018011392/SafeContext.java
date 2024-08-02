package pt.isec.a2018011392;

public class SafeContext {
    ISafeState state;
    SafeData data;

    public SafeContext(SafeData data) {
        this.data = data;
        state = SafeState.CLOSED.getState(this, data);
    }

    public SafeState getState() {
        return state.getState();
    }

    protected void changeState(ISafeState state) {
        this.state = state;
    }


    public boolean isOpen() {
        return state.isOpen();
    }

    public boolean isLocked() {
        return state.isLocked();
    }

    public int getFailedAttempts() {
        return state.getFailedAttempts();
    }

    public double getMoney() {
        return state.getMoney();
    }

    public boolean depositMoney(double amount) {
        return state.depositMoney(amount);
    }

    public boolean withdrawMoney(double amount) {
        return state.withdrawMoney(amount);
    }

    public boolean open(String pin) {
        return state.open(pin);
    }

    public boolean close() {
        return state.close();
    }


    public boolean unlock(String puk) {
        return state.unlock(puk);
    }
}
