package pt.isec.a2018011392;

public interface ISafeState {
    boolean isOpen();
    boolean isLocked();
    int getFailedAttempts();
    double getMoney();
    boolean depositMoney(double amount);
    boolean withdrawMoney(double amount);

    boolean open(String pin);
    boolean close();
    boolean lock();
    boolean unlock(String puk);


    SafeState getState();
}
