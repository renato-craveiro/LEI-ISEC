package pt.isec.a2018011392;

import javax.naming.Context;

abstract class StateAdapter implements IState{
    protected Context context;
    protected SafeDate data;

    public StateAdapter(Context context, SafeDate data) {
        this.context = context;
        this.data = data;
    }
    protected void changeState(IState newState) { context.changeState(newState); }

    public  boolean openAndCorrectCode(){return false;};
    public  boolean openAndWrongCode(){return false;};
    public  boolean close(){return false;};
    public  boolean lock(){return false;};
    public boolean deposit(int amount){return false;};
    public boolean withdraw(int amount){return false;};
    public double getBalance(){return 0;};
    public boolean unlock(int code){return false;};
}
