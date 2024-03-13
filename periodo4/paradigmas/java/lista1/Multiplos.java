import java.util.*;

public class Multiplos {
    public static void main (String[] args) {
        Scanner input = new Scanner(System.in);
        int n = input.nextInt();
        int quantidadeMultiplos = input.nextInt();

        int soma = 0;
        for (int i = 1; i <= quantidadeMultiplos; i++) {
            soma += n*i;
        }
        System.out.println(soma);
        input.close();
    }
}
