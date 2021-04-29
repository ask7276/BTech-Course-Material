/* 
    Author: Kotkar Anket Sanjay
    Roll Number: 180101037

    Commands to execute the program:

    $ javac Assignment1Q2.java
    $ java Assignment1Q2 <number_of_threads>

    for example:
    $ javac Assignment1Q2.java
    $ java Assignment1Q2 7
*/

import java.lang.String;
import java.lang.Math;

public class Assignment1Q2 {

    private double total_function_sum = 0; // weighted function sum of all the endpoints of intervals created
    private int number_of_intervals = 10000000; // number of intervals
    private double interval_length = (double) 2 / (double) number_of_intervals;

    // class extending thread class for the purpose of multithreading
    class Simson extends Thread {

        private int start, end; // the range of points to be considered for particular thread
        private double local_total_function_sum; // weighted function sum of points in range [start, end) among the
                                                 // range [0,10000000]

        public Simson(int s, int e) {
            start = s;
            end = e;
            local_total_function_sum = 0;
            start(); // starting the thread running
        }

        public void run() {
            for (int i = start; i < end; i++) {
                if (i % 2 == 1) {
                    local_total_function_sum += 4 * func(i);
                } else {
                    local_total_function_sum += 2 * func(i);
                }
            }
        }

        // calculating the value of e^(-x^2/2)
        public double func(int i) {
            double d = -1 + interval_length * (double) i;
            d = d * d;
            d /= 2;
            double ans = Math.exp(-d);
            return ans;
        }

        public double getSum() {
            return local_total_function_sum;
        }
    }

    public static void main(String[] args) {

        try {
            Assignment1Q2 assignment1q2 = new Assignment1Q2();

            if (Integer.parseInt(args[0]) < 4 || Integer.parseInt(args[0]) > 16) {
                throw new Exception();
            }

            System.out.println("Number of threads created " + args[0]);
            assignment1q2.count(args);
        } catch (ArrayIndexOutOfBoundsException e) {
            System.out.println("Number of threads to be generated was not provided.");
        } catch (Exception e) {
            System.out.println("Number of threads should be between 4 to 16. Error while executing the program.");
        }
    }

    public void count(String[] args) {

        int num_of_threads = Integer.parseInt(args[0]); // number of threads created

        // threads generated - storing them in array of class extending thread class
        Simson threads[] = new Simson[num_of_threads];

        int start = 1;
        int temp = (number_of_intervals - 1) / num_of_threads;

        for (int i = 0; i < num_of_threads; i++) {
            if (i < (number_of_intervals - 1) % num_of_threads) {
                threads[i] = new Simson(start, start + temp + 1);
                start = start + temp + 1;
            } else {
                threads[i] = new Simson(start, start + temp);
                start = start + temp;
            }
        }

        // checking whether any of the thread is alive
        for (int i = 0; i < num_of_threads; i++) {
            while (threads[i].isAlive())
                ;
        }

        total_function_sum = 0;
        for (int i = 0; i < num_of_threads; i++) {
            total_function_sum += threads[i].getSum();
        }
        total_function_sum += 2 * Math.exp(0.5); // adding function values of endpoints namely 1 and -1
        System.out.println("Number of intervals created are " + number_of_intervals);

        // calculate integral
        calculateIntegral(num_of_threads);
    }

    public void calculateIntegral(int num_of_threads) {

        double d = 2 * total_function_sum / ((double) 3 * number_of_intervals);
        d = d / (Math.sqrt(2 * Math.PI)); // multiplying the function sum by constant 1/(sqrt(2*PI)) to get integral
                                          // value
        System.out.println("Calculated value of integral is " + d);
    }
}
