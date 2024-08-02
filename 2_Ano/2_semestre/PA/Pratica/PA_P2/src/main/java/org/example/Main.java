package org.example;


import static org.example.Ex2.guessGame;

public class Main {
    public static void ex2(){
        System.out.println("Ex. 2: Guess the number game\n");
        guessGame();
        System.out.println("\n=========================\n\n");
    }

    public static void ex4(){
        System.out.println("Ex. 4: Random array\n");
        Ex4 ex4 = new Ex4();
        ex4.printArray();
        System.out.println("\n=========================\n\n");
    }

    public static void ex7(){
        System.out.println("Ex. 7: Matrix sum\n");
        Ex7 ex7 = new Ex7(3,3);
        ex7.sum();
        System.out.println("-------------------------------------------------");
        RectMatrix m1 = new RectMatrix(2,2);
        RectMatrix m2 = new RectMatrix(2,2);
        Ex7 ex7_2 = new Ex7(m1,m2);
        ex7_2.sum();
        System.out.println("-------------------------------------------------");
        Ex7.sum(m1,m2);
        System.out.println("\n=========================\n\n");
    }

    public static void ex9(){
        System.out.println("Ex. 9: Pascal's Triangle\n");
        Ex9PascalTriangle ex9PascalTriangle = new Ex9PascalTriangle(5);
        ex9PascalTriangle.printTriangleV1();
        ex9PascalTriangle.printTriangleV2();
        System.out.println("\n=========================\n\n");
    }

    public static void presentation(){
        System.out.println("\n\n=================================================");
        System.out.println("[ISEC: LEI-RPL - 2nd year- 2nd Semester]\n" +
                "\nAdvanced Programming - Practical Class no. 2");
        System.out.println("=================================================\n\n");
    }



    public static void main(String[] args) {
        presentation();

        ex2();

        ex4();

        ex7();

        ex9();
    }
}