package pt.isec.a2018011392;

import java.util.List;

// Press Shift twice to open the Search Everywhere dialog and type `show whitespaces`,
// then press Enter. You can now see whitespace characters in your code.
public class Main {
    public static void main(String[] args) throws CloneNotSupportedException {
        //Lists
        /*
        Library library = new Library("LOOOOL");
        library.addBook("LOL", List.of("LOL","XD","Drogas pesadas"));
        library.addBook("LOL 2ª ed.", List.of("LOL","XD","Drogas não tão pesadas"));
        library.addBook("LOL 3ª ed.", List.of("LOL"));
        System.out.println(library.findBook("LOL").toString());
        System.out.println(library.findBook(1).toString());
        System.out.println(library);
        System.out.println(library.removeBook("LOL")+ "----------------------");
        System.out.println(library);
        */

        //Sets
        /*LibrarySet library = new LibrarySet("LOOOOL");
        library.addBook("LOL", List.of("LOL","XD","Drogas pesadas"));
        library.addBook("LOL 2ª ed.", List.of("LOL","XD","Drogas não tão pesadas"));
        library.addBook("LOL 3ª ed.", List.of("LOL"));
        System.out.println(library.findBook("LOL").toString());
        System.out.println(library.findBook(1).toString());
        System.out.println(library);
        System.out.println(library.removeBook("LOL")+ "----------------------");
        System.out.println(library);
        */

        //Maps
        /*LibraryMap library = new LibraryMap("LOOOOL");
        library.addBook("LOL", List.of("LOL","XD","Drogas pesadas"));
        library.addBook("LOL 2ª ed.", List.of("LOL","XD","Drogas não tão pesadas"));
        library.addBook("LOL 3ª ed.", List.of("LOL"));
        System.out.println(library.findBook("LOL").toString());
        System.out.println(library.findBook(1).toString());
        System.out.println(library);
        System.out.println(library.removeBook("LOL")+ "----------------------");
        System.out.println(library);*/

        //user Interface
        //UserInterface.main(null);



        Book livroVelho = new OldBook("LOL", List.of("LOL","XD","Drogas pesadas"));
        Book livroNovo = new RecentBook("LOL", List.of("LOL","XD","Drogas pesadas"));
        if(livroVelho.equals(livroNovo))
            System.out.println("Iguais");
        else
            System.out.println("Diferentes");



    }
}