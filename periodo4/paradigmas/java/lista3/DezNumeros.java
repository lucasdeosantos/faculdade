import java.util.Scanner;

public class DezNumeros {
    public static void main (String[] args) {
        Scanner input = new Scanner(System.in);

        int somaPares = 0;
        int quantidadeImpares = 0;

        for (int i = 0; i < 10; i++) {
            int n = input.nextInt();

            if (n % 2 == 0) {
                somaPares += n;
                System.out.println(n + " é um número par.");
            }
            else {
                quantidadeImpares += 1;
                System.out.println(n + " é um número ímpar.");
            }
        }
        System.out.println("A soma dos números pares é " + somaPares + ".");
        System.out.println("A quantidade de números impares é " + quantidadeImpares + ".");
    }
}