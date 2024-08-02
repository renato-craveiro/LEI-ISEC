package org.example;

import java.util.Random;
import java.util.Scanner;

import static java.lang.System.exit;

public class Ex2 {

    public static void guessGame() {
        Scanner sc = new Scanner(System.in);
        int i;
        // Press Alt+Enter with your caret at the highlighted text to see how
        // IntelliJ IDEA suggests fixing it.
        System.out.printf("Hello and welcome!");

        //random number between 0 and 100
        //int random=(int) (Math.random() * 100);;
        String input;
        Random r = new Random();
        int low = 1;
        int high = 100;
        int random = r.nextInt(high - low) + low;

        do {


            System.out.println("Is the number " + random + "?");
            input = sc.nextLine();
            try {
                switch (input) {
                    case "higher" -> {
                        low = random;
                        random = r.nextInt(high - low) + low;
                    }
                    case "lower" -> {
                        high = random;
                        random = r.nextInt(high - low) + low;
                    }

                    case "correct" -> System.out.println("I won!");

                    case "exit" -> {
                        System.out.println("lol, you gave up....");
                        return;
                    }

                    default -> System.out.println("I don't understand");
                }

                //System.out.println("low: " + low + " high: " + high);
            } catch (IllegalArgumentException e) {
                System.out.println("Try to fool me! I'm not that easy to fool!");
            }

        } while (!input.equalsIgnoreCase("correct"));


    }


}
