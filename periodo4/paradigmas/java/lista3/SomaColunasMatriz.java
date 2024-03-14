import java.util.Scanner;

public class SomaColunasMatriz {
    public static void main(String[] args) {
        Scanner input = new Scanner(System.in);
        System.out.print("Numero de linhas: ");
        int linhas = input.nextInt();
        System.out.print("Numero de colunas: ");
        int colunas = input.nextInt();
        int[][] matriz = lerMatriz(linhas, colunas);

        int[] resultados = somaColunaMatriz(matriz, linhas, colunas);

        System.out.println("Vetor resultante:");
        for (int resultado : resultados)
            System.out.printf("%d ", resultado);
        System.out.println();
    }

    public static int[][] lerMatriz(int linhas, int colunas) {
        Scanner input = new Scanner(System.in);
        int[][] matriz = new int[linhas][colunas];

        System.out.println("Digite os elementos da matriz:");
        for (int i = 0; i < linhas; i++)
            for (int j = 0; j < colunas; j++)
                matriz[i][j] = input.nextInt();
            
        return matriz;
    }

    public static int[] somaColunaMatriz(int[][] matriz, int linhas, int colunas) {
        int[] resultados = new int[linhas];

        for (int i = 0; i < linhas; i++)
            for (int j = 0; j < colunas; j++)
                resultados[i] += matriz[j][i];

        return resultados;
    }
}
