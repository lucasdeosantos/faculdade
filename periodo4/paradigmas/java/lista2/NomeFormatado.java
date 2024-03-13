import java.util.Scanner;

public class NomeFormatado {
    public static void main (String[] args) {
        Scanner input = new Scanner(System.in);
        String nomeCompleto = input.nextLine();

        String[] nomes = nomeCompleto.split(" ");
        char inicial;
        String sobrenome;
        
        if (nomes.length >= 2) {
            sobrenome = nomes[nomes.length - 1];
            inicial = nomeCompleto.charAt(0);
            System.out.printf("%s, %c.\n", sobrenome, inicial);
        }
    }
}
