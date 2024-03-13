import java.util.Scanner;

public class Idades {
    public static void main (String[] args) {
        Scanner input = new Scanner(System.in);

        int[] idades = new int[10];
        int total = 0;

        for (int i = 0; i < 10; i++) {
            idades[i] = input.nextInt();
            total += idades[i];
        }
        
        for (int i = 0; i < 10; i++) {
            if (idades[i] > (total / 10))
                System.out.println("Acima da média.");
            else if (idades[i] < (total / 10))
                System.out.println("Abaixo da média.");
            else
                System.out.println("Na média.");
        }

        input.close();
    }
}
