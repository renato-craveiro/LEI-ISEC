package pt.isec.a2018011392;

public class SafeData {

    private static final String CODE_PIN ="1234";
    private static final String CODE_PUK ="123456";

    private static final int MAX_FAILED_ATTEMPTS = 3;

    boolean closed;
    boolean locked;
    int failedAttempts;
    double money;

    public SafeData(int failedAttempts, double money) {
        this.locked = false;
        this.closed = true;
        this.failedAttempts = failedAttempts;
        this.money = money;
    }

    public boolean isOpen() {
        return !locked;
    }
    public boolean isLocked() {

        return locked;
    }

    public int getFailedAttempts() {
        return failedAttempts;
    }

    public double getMoney() {
        return money;
    }

    public boolean depositMoney(double amount) {
        if (locked) {
            return false;
        }
        money += amount;
        return true;
    }

    public boolean withdrawMoney(double amount) {
        if (locked) {
            return false;
        }
        if (money < amount) {
            return false;
        }
        if(closed) {
            return false;
        }
        money -= amount;
        return true;
    }


    public boolean open(String code) {
        if (locked) {
            return false;
        }
        if (code.equals(CODE_PIN)) {
            closed = false;
            failedAttempts = 0;
            return true;
        }
        failedAttempts++;
        System.out.println("Failed attempts: " + failedAttempts);
        if (failedAttempts >= MAX_FAILED_ATTEMPTS) {
            System.out.println("Too many failed attempts");
            locked = true;
        }
        return false;

    }

    public boolean close() {
        if (locked) {
            return false;
        }
        closed = true;
        return true;

    }

    public boolean unlock(String code) {
        if (!locked) {
            return false;
        }
        if (code.equals(CODE_PUK)) {
            locked = false;
            failedAttempts = 0;
            return true;
        }
        return false;
    }

    public boolean lock() {
        if (locked) {
            return false;
        }

            locked = true;
            return true;
    }
}
