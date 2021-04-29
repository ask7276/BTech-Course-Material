/* 
    Author: Kotkar Anket Sanjay
    Roll Number: 180101037

    Commands to execute the program:

    $ javac Assignment1Q3.java
    $ java Assignment1Q3 <number_of_threads>

    for example:
    $ javac Assignment1Q3.java
    $ java Assignment1Q3 7
*/

import java.lang.String;
import java.lang.Math;

public class Assignment1Q3 {

    private int rows = 1000;
    private int columns = 1000;
    private double[][] matrix_A = new double[rows][columns];
    private double[][] matrix_B = new double[rows][columns];
    private double[][] matrix_C = new double[rows][columns];

    // class extending thread class for the purpose of initialization by
    // multithreading
    class Initialization extends Thread {

        private int start, end; // the range of rows to be considered for particular thread

        public Initialization(int s, int e) {
            start = s;
            end = e;
            start(); // starting the thread running
        }

        public void run() {
            for (int i = start; i < end; i++) {
                for (int j = 0; j < columns; j++) {
                    matrix_A[i][j] = Math.random() * 10.0;
                    matrix_B[i][j] = Math.random() * 10.0;
                }
            }
        }
    }

    // class extending thread class for the purpose of matrix multiplication by
    // multithreading
    class MatrixMul extends Thread {

        private int start, end; // the range of rows to be considered for particular thread

        public MatrixMul(int s, int e) {
            start = s;
            end = e;
            start(); // starting the thread running
        }

        public void run() {
            for (int i = start; i < end; i++) {
                for (int j = 0; j < columns; j++) {
                    double d = calculateValue(i, j);
                    matrix_C[i][j] = d;
                }
            }
        }

        public double calculateValue(int i, int j) {
            double d = 0;
            for (int k = 0; k < rows; k++) {
                d += matrix_A[i][k] * matrix_B[k][j];
            }
            return d;
        }
    }

    public static void main(String[] args) {

        try {
            Assignment1Q3 assignment1q3 = new Assignment1Q3();

            if (Integer.parseInt(args[0]) < 4 || Integer.parseInt(args[0]) > 16) {
                throw new Exception();
            }

            System.out.println("Number of threads created " + args[0]);
            long startTime = System.currentTimeMillis();
            assignment1q3.initialize(args);
            assignment1q3.calculate(args);
            long endTime = System.currentTimeMillis();
            System.out.print(
                    "Matrices were initializad with random double values between 0 to 10 and multiplied.\nTime taken by program to execute is "
                            + String.valueOf(endTime - startTime) + "ms\n");
        } catch (ArrayIndexOutOfBoundsException e) {
            System.out.println("Number of threads to be generated was not provided.");
        } catch (Exception e) {
            System.out.println("Number of threads should be between 4 to 16. Error while executing the program.");
        }
    }

    public void initialize(String[] args) {

        int num_of_threads = Integer.parseInt(args[0]); // number of threads created

        // threads generated - storing them in array of class extending thread class
        Initialization threads[] = new Initialization[num_of_threads];

        int start = 0;
        int temp = (rows) / num_of_threads;

        for (int i = 0; i < num_of_threads; i++) {
            if (i < (rows) % num_of_threads) {
                threads[i] = new Initialization(start, start + temp + 1);
                start = start + temp + 1;
            } else {
                threads[i] = new Initialization(start, start + temp);
                start = start + temp;
            }
        }

        // checking whether any of the thread is alive
        for (int i = 0; i < num_of_threads; i++) {
            while (threads[i].isAlive())
                ;
        }
    }

    public void calculate(String[] args) {

        int num_of_threads = Integer.parseInt(args[0]); // number of threads created

        // threads generated - storing them in array of class extending thread class
        MatrixMul threads[] = new MatrixMul[num_of_threads];

        int start = 0;
        int temp = (rows) / num_of_threads;

        for (int i = 0; i < num_of_threads; i++) {
            if (i < (rows) % num_of_threads) {
                threads[i] = new MatrixMul(start, start + temp + 1);
                start = start + temp + 1;
            } else {
                threads[i] = new MatrixMul(start, start + temp);
                start = start + temp;
            }
        }

        // checking whether any of the thread is alive
        for (int i = 0; i < num_of_threads; i++) {
            while (threads[i].isAlive())
                ;
        }
    }
}
