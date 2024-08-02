package org.example;

public class Ex9PascalTriangle {
    int prof;

    int factorial(int n) {
        if (n == 0 || n == 1) {
            return 1;
        }
        return n * factorial(n - 1);
    }

    private int triangle[][];

    Ex9PascalTriangle(int prof) {
        this.prof = prof;
        triangle = new int[prof][prof];

        for (int i = 0; i < prof; i++) {
            for (int j = 0; j <= i; j++) {
                triangle[i][j] = factorial(i) / (factorial(j) * factorial(i - j));
            }
        }
    }

    String traingleStringv1() {
        StringBuilder sb = new StringBuilder();
        for (int i = 0; i < prof; i++) {
            for (int j = 0; j <= i; j++) {
                sb.append(triangle[i][j]).append(" ");
            }
            sb.append("\n");
        }
        return sb.toString();
    }

    String traingleStringv2() {
        StringBuilder sb = new StringBuilder();
        for (int i = 0; i < prof; i++) {
            for (int j = 0; j < prof - i; j++) {
                sb.append(" ");
            }
            for (int j = 0; j <= i; j++) {
                sb.append(triangle[i][j]).append(" ");
            }
            sb.append("\n");
        }
        return sb.toString();
    }

    void printTriangleV1() {
        System.out.println(traingleStringv1());
    }

    void printTriangleV2() {
        System.out.println(traingleStringv2());
    }

}
