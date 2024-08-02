package pt.isec.pa.javalife.fsmStudyGumball.model.data;

public class GumballMachineData {
    private int count = 0;
    public GumballMachineData(int count) { this.count = count; }
    public int getCount() { return count; }
    public void refillGumballs(int count) {
        if (count>0)
            this.count += count;
    }
    public boolean getGumball() {
        if (count>0) {
            count--;
            return true;
        }
        return false;
    }
    @Override
    public String toString() {
        return String.format("Gumball Machine with %d gumball(s)",count);
    }
}