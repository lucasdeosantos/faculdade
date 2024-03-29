package cartao;

import java.util.Scanner;

public class Main {
    public static void main(String[] args) {
        Scanner input = new Scanner(System.in);

        System.out.println("Escreva o nome do rementente:");
        String rementente = input.nextLine();

        System.out.println("Escreva o nome do destinatário:");
        String destinatario = input.nextLine();

        CartaoWeb[] cartoes = new CartaoWeb[3];
        cartoes[0] = new CartaoDiaDosNamorados(destinatario);
        cartoes[1] = new CartaoAniversario(destinatario);
        cartoes[2] = new CartaoNatal(destinatario);

        for (CartaoWeb cartao : cartoes) {
            cartao.retornarMensagem(rementente);
            System.out.println();
        }
    }
}
