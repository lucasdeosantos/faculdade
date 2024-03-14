import java.util.Scanner;

public class BuscaMatriz {
    public static void main(String[] args) {
        Scanner input = new Scanner(System.in);
        int[][] matriz = lerMatriz(4, 4);

        System.out.println("digite o elemento procurado:");
        int elemento = input.nextInt();

        boolean localizou = buscaMatriz(matriz, 4, 4, elemento);

        if (!localizou)
            System.out.println("elemento não encontrado.");
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

    public static boolean buscaMatriz(int[][] matriz, int linhas, int colunas, int elemento) {
        for(int i = 0; i < linhas; i++) {
            for(int j = 0; j < colunas; j++) {
                if (matriz[i][j] == elemento) {
                    System.out.printf("elemento localizado em [%d][%d]\n", i, j);
                    return true;
                }
            }
        }
        return false;
    }
}
