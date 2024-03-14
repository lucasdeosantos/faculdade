import java.util.Scanner;

public class Vogais {
    public static void main(String[] args) {
        Scanner input = new Scanner(System.in);

        String palavra = input.nextLine();
        int contadorVogais = 0;

        for (int i = 0; i < palavra.length(); i++) {
            char c = palavra.charAt(i);
            if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') {
                contadorVogais++;
            }
        }
        System.out.println("Numero de vogais: " + contadorVogais);
        System.out.println(palavra.replace('a', 'r'));
    }
}