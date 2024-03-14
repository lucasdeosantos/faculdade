import java.util.Scanner;

public class Invertidas {
    public static void main(String[] args) {
        Scanner input = new Scanner(System.in);

        String frase1 = input.nextLine();
        String frase2 = input.nextLine();

        System.out.println(inverterString(frase1));
        System.out.println(inverterString(frase2));
    }

    public static String inverterString(String frase) {
        StringBuilder invertida = new StringBuilder(frase);
        invertida.reverse();
        return invertida.toString().replace('A', '*');
    }
}
