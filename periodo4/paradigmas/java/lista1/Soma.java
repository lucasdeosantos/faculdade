import java.util.*;

public class Soma {
    public static void main (String[] args) {
        Scanner input = new Scanner(System.in);

        int n = input.nextInt();
        int soma = 0;

        while (n != -1) {
            soma += n;
            n = input.nextInt();
        }
        System.out.println("Soma = " + soma);
    }
}
