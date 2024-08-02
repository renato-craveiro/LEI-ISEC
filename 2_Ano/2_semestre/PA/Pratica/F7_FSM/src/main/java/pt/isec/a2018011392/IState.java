package pt.isec.a2018011392;

public interface IState {


    public  boolean openAndCorrectCode();
    public  boolean close();
    public  boolean lock();
    public  boolean openAndWrongCode();

    public boolean deposit(int amount);
    public boolean withdraw(int amount);
    public double getBalance();

    public boolean unlock(int code);

    State getState();
}
