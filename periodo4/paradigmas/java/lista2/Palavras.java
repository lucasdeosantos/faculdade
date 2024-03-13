import java.util.Scanner;

public class Palavras {
    public static void main (String[] args) {
        Scanner input = new Scanner(System.in);
        String frase = input.nextLine();

        int palavras;
        if (frase.charAt(0) == ' ')
            palavras = 0;
        else
            palavras = 1;

        for (int i = 0; i < frase.length(); i++)
            if (frase.charAt(i) == ' ')
                palavras++;

        System.out.println("A frase contem " + palavras + " palavras.");
    } 
}
