import java.util.*;

public class Triangulo {
    public static void main (String[] args) {
        Scanner input = new Scanner(System.in);

        System.out.println("Digite os valores correspondentes aos lados a, b e c respectivamente:");

        int a = input.nextInt();
        int b = input.nextInt();
        int c = input.nextInt();

        if (verificarTriangulo(a, b,c )) {
            if (equilatero(a, b, c))
                System.out.println("É um triangulo equilatero.");
            else if (isosceles(a, b, c))
                System.out.println("É um triangulo isosceles.");
            else
                System.out.println("É um triangulo escaleno.");
        }
        else {
            System.out.println("Não é um triangulo");
        }
    }
    private static boolean verificarTriangulo(int a, int b, int c) {
        return (a + b > c) && (b + c > a) && (c + a > b);
    }
    private static boolean equilatero(int a, int b, int c) {
        return a == b && b == c;
    }
    private static boolean isosceles(int a, int b, int c) {
        return a == b || b == c || c == a;
    }
    // Não é necessário.
    // private static boolean escaleno(int a, int b, int c) {
    //    return a != b && b != c;
    // }
}
