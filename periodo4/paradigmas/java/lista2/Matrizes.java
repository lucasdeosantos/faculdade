import java.util.Scanner;

public class Matrizes {
    public static void main(String[] args) {
        int linhas = 3, colunas = 3;

        float[][] matrizA = new float[linhas][colunas];
        float[][] matrizB = new float[linhas][colunas];
        float[][] soma =  new float[linhas][colunas];

        matrizA = lerMatriz(linhas, colunas);
        matrizB = lerMatriz(linhas, colunas);

        soma = somaMatrizes(matrizA, matrizB, linhas, colunas);

        imprimirMatriz(soma, linhas, colunas);
    }

    public static float[][] lerMatriz(int linhas, int colunas) {
        Scanner input = new Scanner(System.in);
        float[][] matriz = new float[linhas][colunas];

        System.out.println("Digite os elementos da matriz:");
        for (int i = 0; i < linhas; i++)
            for (int j = 0; j < colunas; j++)
                matriz[i][j] = input.nextFloat();
            
        return matriz;
    }

    public static void imprimirMatriz(float[][] matriz, int linhas, int colunas) {
        for (int i = 0; i < linhas; i++) {
            for (int j = 0; j < colunas; j++) {
                System.out.printf("[%.2f]", matriz[i][j]);
            }
            System.out.println();
        }
    }

    public static float[][] somaMatrizes(float[][] matrizA, float[][] matrizB, int linhas, int colunas) {
        float[][] soma = new float[linhas][colunas];

        for (int i = 0; i < linhas; i++)
            for (int j = 0; j < colunas; j++)
                soma[i][j] = matrizA[i][j] + matrizB[i][j];

        return soma;
    }
}
