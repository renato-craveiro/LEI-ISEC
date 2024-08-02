package pt.isec.a2018011392.hangman;

public class HangmanGame {
    public static void main(String args[]) {
        HangmanModel game = new HangmanModel();
        HangmanUI gameUI = new HangmanUI(game);
        gameUI.play();
    }
}
