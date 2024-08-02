package pt.isec.a2018011392;

public class SafeDate {
    private Double money;
    private int code; //4digit

    int attempts = 0;

    public SafeDate(Double money, int code) {
        this.money = money;
        this.code = code;
    }

    public SafeDate(Double money) {
        this.money = money;
        code = 1234;
    }

    public boolean widthdraw(Double money){
        if(this.money >= money){
            this.money -= money;
            return true;
        }
        return false;
    }

    public boolean deposit(Double money){
        this.money += money;
        return true;
    }

    public double getBalance(){
        return money;
    }

    public int getCode() {
        return code;
    }

    public void setCode(int code) {
        this.code = code;
    }

    public Double getMoney() {
        return money;
    }

    public void setMoney(Double money) {
        this.money = money;
    }

    public void resetAttempts(){
        attempts = 0;
    }

    public void incrementAttempts(){
        attempts++;
    }


}
