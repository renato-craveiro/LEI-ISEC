package pt.isec.a2018011392;

import pt.isec.a2018011392.ui.SafeUI;

// Press Shift twice to open the Search Everywhere dialog and type `show whitespaces`,
// then press Enter. You can now see whitespace characters in your code.
public class Main {
    public static void main(String[] args) {
        // Create a new SafeData object
        SafeData data = new SafeData(0,2000);
        // Create a new SafeContext object with the SafeData object
        SafeContext context = new SafeContext(data);

        SafeUI ui = new SafeUI(context);
        ui.start();

    }
}