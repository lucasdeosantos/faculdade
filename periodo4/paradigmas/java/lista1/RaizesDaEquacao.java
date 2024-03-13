import java.util.*;

public class RaizesDaEquacao {
    public static void main (String[] args) {
        Scanner input = new Scanner(System.in);

        System.out.println("Digite os valores correspondentes as variavies a, b e c respectivamente:");

        int a = input.nextInt();
        int b = input.nextInt();
        int c = input.nextInt();

        double[] resultado = new double[2]; 
        
       if (raizesEquacaoSegundoGrau(resultado, a, b, c)) {
            for(int i = 0; i < resultado.length; i++) {
                if (resultado[i] % 1 == 0)
                    System.out.printf("x%d = %.0f\n", i + 1, resultado[i]);
                else
                    System.out.printf("x%d = %.1f\n", i + 1, resultado[i]);       
            }
       }
       else {
            System.out.println("Equação não possui raizes reais.");
       }
    }
    private static boolean raizesEquacaoSegundoGrau(double[] resultado, int a, int b, int c) {
        double delta = Math.pow(b, 2) - (4 * a * c);

        if (delta > 0) {
            double x1 = (-b + Math.sqrt(delta)) / (2 * a);
            resultado[0] = x1;
            double x2 = (-b - Math.sqrt(delta)) / (2 * a);
            resultado[1] = x2;
            return true;
        }
        return false;
    }
}
