package pt.isec.a2018011392;

public class OpenState extends SafeStateAdapter {
    public OpenState(SafeContext context, SafeData data) {
        super(context, data);
    }

    @Override
    public double getMoney() {
        return data.getMoney();
    }

    @Override
    public boolean depositMoney(double amount) {
        return data.depositMoney(amount);
    }

    @Override
    public boolean withdrawMoney(double amount) {
        return data.withdrawMoney(amount);
    }

    @Override
    public boolean close() {
        if(data.close()) {
            changeState(new ClosedState(context, data));
            return true;
        }
        return false;
    }

    @Override
    public SafeState getState() {
        return SafeState.OPEN;
    }
}
