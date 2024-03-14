import java.util.Scanner;

public class VetorMenorMaior {
    public static void main (String[] args) {
        Scanner input = new Scanner(System.in);
        
        int tamanhoVetor = 7;
        int[] vetor = new int[tamanhoVetor];
        vetor[0] = input.nextInt();

        int menor, maior;
        menor = vetor[0];
        maior = vetor[0];

        for (int i = 1; i < tamanhoVetor; i++) {
            vetor[i] = input.nextInt();

            if (vetor[i] < menor)
                menor = vetor[i];

            if (vetor[i] > maior)
                maior = vetor[i];
        }
        System.out.println("menor: " + menor);
        System.out.println("maior: " + maior);
    }
}
