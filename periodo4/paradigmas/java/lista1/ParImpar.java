import java.util.*;

public class ParImpar {
    public static void main (String[] args) {
        Scanner input = new Scanner(System.in);

        int n = input.nextInt();

        if (n == 0)
            System.out.println("Zero");
        else if (n % 2 == 0)
            System.out.println("Par");
        else
            System.out.println("Impar");
    } 
}
