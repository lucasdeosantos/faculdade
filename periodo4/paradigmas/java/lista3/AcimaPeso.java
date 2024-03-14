import java.util.Scanner;

public class AcimaPeso {
    public static void main (String[] args) {
        Scanner input = new Scanner(System.in);

        Float altura = input.nextFloat();
        Float peso = input.nextFloat();
        int acimaPeso = 0;
        float error = 0.0001f;

        while (altura != -1 && peso != -1) {
            if (((peso / (altura*altura)) - 25.0f) > error) {
                System.out.println("Acima do peso.");
                acimaPeso++;
            }

            altura = input.nextFloat();
            peso = input.nextFloat();
        }

        System.out.println(acimaPeso + " pessoas estão acima do peso.");
    }
}
