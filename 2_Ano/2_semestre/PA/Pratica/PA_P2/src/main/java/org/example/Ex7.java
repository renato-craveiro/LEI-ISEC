package org.example;

public class Ex7 {
    RectMatrix m1, m2;

    public Ex7(int rows, int cols) {
        m1 = new RectMatrix(rows, cols);
        m2 = new RectMatrix(rows, cols);
        m1.fillMatrix();
        m2.fillMatrix();
    }

    public Ex7(RectMatrix m1, RectMatrix m2) {
        this.m1 = m1;
        this.m2 = m2;
    }

    public void sum() {
        System.out.println("Sum of the two matrices: ");
        m1.printMatrix();
        System.out.println("+");
        m2.printMatrix();
        System.out.println("=");
        if (checkDiffDimensions(m1, m2)) {
            System.out.println("Matrices have different dimensions");
            return;
        }
        RectMatrix result = new RectMatrix(m1.getRows(), m1.getCols());
        for (int i = 0; i < m1.getRows(); i++) {
            for (int j = 0; j < m1.getCols(); j++) {
                result.matrix[i][j] = m1.matrix[i][j] + m2.matrix[i][j];
            }
        }
        result.printMatrix();
    }

    private static boolean checkDiffDimensions(RectMatrix m1, RectMatrix m2) {
        return m1.getRows() != m2.getRows() || m1.getCols() != m2.getCols();
    }

    public static void sum(RectMatrix m1, RectMatrix m2) {
        System.out.println("Sum of the two matrices: ");
        m1.printMatrix();
        System.out.println("+");
        m2.printMatrix();
        System.out.println("=");
        if (checkDiffDimensions(m1, m2)) {
            System.out.println("Matrices have different dimensions");
            return;
        }
        RectMatrix result = new RectMatrix(m1.getRows(), m1.getCols());
        for (int i = 0; i < m1.getRows(); i++) {
            for (int j = 0; j < m1.getCols(); j++) {
                result.matrix[i][j] = m1.matrix[i][j] + m2.matrix[i][j];
            }
        }
        result.printMatrix();
    }

}
