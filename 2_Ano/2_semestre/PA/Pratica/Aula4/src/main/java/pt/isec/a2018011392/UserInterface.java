package pt.isec.a2018011392;

import java.util.Arrays;
import java.util.Scanner;

public class UserInterface {
    private static void setAndPrintLibraryData(Scanner scanner, ILibrary library, Book b) {
        System.out.println(library.getName());

        System.out.println("Title: ");
        String title = scanner.nextLine();

        System.out.println("Authors: ");
        String authors = scanner.nextLine();
        String[] authorsArray = authors.split(",");

        library.addBook(title, Arrays.stream(authorsArray).toList(),b);
        System.out.println(library);
    }

    private static void libLists(Book b){
        Scanner scanner = new Scanner(System.in);
        ILibrary library = new Library("LibraryList");
        setAndPrintLibraryData(scanner, library,b);

    }

    private static void libMap(Book b) {
        Scanner scanner = new Scanner(System.in);

        ILibrary libraryMap = new LibraryMap("LibraryMap");
        setAndPrintLibraryData(scanner, libraryMap,b);
    }

    private static void libSet(String[] args,Book b) {
        Scanner scanner = new Scanner(System.in);
        ILibrary librarySet = new LibrarySet("LibrarySet");
        setAndPrintLibraryData(scanner, librarySet,b);

    }


    public static void main(String[] args) {
        Book book2add = null;
        Scanner scanner = new Scanner(System.in);

        System.out.println("Select the type of book you want to use: ");
        System.out.println("1 - Old");
        System.out.println("2 - Recent");
        int option = 0;

        while (!scanner.hasNextInt())
            scanner.next();
         option = scanner.nextInt();
        switch (option) {
            case 1 -> book2add = new OldBook("LOL", Arrays.stream(new String[]{"LOL", "XD", "Drogas pesadas"}).toList());
            case 2 -> book2add = new RecentBook("LOL", Arrays.stream(new String[]{"LOL", "XD", "Drogas pesadas"}).toList());
            default -> book2add = new OldBook("LOL", Arrays.stream(new String[]{"LOL", "XD", "Drogas pesadas"}).toList());
        }

        book2add = new OldBook("LOL", Arrays.stream(new String[]{"LOL", "XD", "Drogas pesadas"}).toList());
        System.out.println("Select the type of library you want to use: ");
        System.out.println("1 - List");
        System.out.println("2 - Map");
        System.out.println("3 - Set");
        System.out.println("4 - Exit");
        option = 0;
        while (option != 4) {
            while(!scanner.hasNextInt())
                scanner.next();
            option = scanner.nextInt();
            switch (option) {
                case 1 -> libLists(book2add);
                case 2 -> libMap(book2add);
                case 3 -> libSet(args,book2add);
                case 4 -> System.out.println("Exiting...");
                default -> System.out.println("Invalid option");
            }
        }
    }

}
