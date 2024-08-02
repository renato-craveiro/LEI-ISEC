package org.example;

public class Ex4 {
    private int arr[] = new int[20];
    private int repeatedCounter = 0;

    Ex4() {
        for (int i = 0; i < 20; i++) {
            int n = (int) (Math.random() * 100); //random number between 0 and 100
            while (checkExist(n)) {
                repeatedCounter++;
                n = (int) (Math.random() * 100);
            }
            arr[i] = n;
        }
    }

    public void printArray() {
        for (int j : arr) {
            System.out.print(j + " ");
        }
        System.out.println();
        System.out.println("Repeated numbers: " + repeatedCounter);
    }


    private boolean checkExist(int n) {
        for (int j : arr) {
            if (j == n) {
                return true;
            }
        }
        return false;
    }

}
