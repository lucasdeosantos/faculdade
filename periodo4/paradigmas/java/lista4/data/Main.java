import java.util.Scanner;

public class Main {
    public static void main(String[] args) {
        Scanner input = new Scanner(System.in);

        Data[] datas = new Data[3];

        for (int i = 0; i < 3; i++) {
            datas[i] = new Data();
            datas[i].setData(input.nextInt(), input.nextInt(), input.nextInt());
        }

        Data menor = datas[0];
        Data maior = datas[0];

        for (int i = 1; i < 3; i++) {
            if (menor.anterior(datas[i].getDia(), datas[i].getMes(), datas[i].getAno()))
                menor = datas[i];

            if (maior.posterior(datas[i].getDia(), datas[i].getMes(), datas[i].getAno()))
                maior = datas[i];
        }

        System.out.println("a data mais antiga é " + menor.toString() + ".");
        System.out.println("a data mais atual é " + maior.toString() + ".");
    }
}
