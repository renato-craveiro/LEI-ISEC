package org.example;

import java.util.Random;

//PART OF EX 7
public class RectMatrix {
    protected int[][] matrix;

    public int getRows() {
        return rows;
    }

    public int getCols() {
        return cols;
    }

    private int rows;
    private int cols;

    public RectMatrix(int rows, int cols) {
        this.rows = rows;
        this.cols = cols;
        matrix = new int[rows][cols];
        fillMatrix();
        //System.out.println("Orig Matrix " + rows + "x" + cols + " created");
        //printMatrix();
    }

    public void fillMatrix() {
        Random r = new Random();
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                matrix[i][j] = r.nextInt(100);
            }
        }
    }

    public void printMatrix() {
        for (int[] row : matrix) {
            for (int j : row) {
                System.out.print(j + " ");
            }
            System.out.println();
        }
    }
}
