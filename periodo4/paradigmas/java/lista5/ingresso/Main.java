package ingresso;

import java.util.Random;

public class Main {
    public static void main(String[] args) {
        IngressoNormal[] normais = new IngressoNormal[25];
        IngressoVip[] vips = new IngressoVip[25];
        IngressoCamarote[] camarotes = new IngressoCamarote[25];
        
        Random random = new Random();
        for (int i = 0; i < 25; ++i) {
            double valorIngresso = random.nextDouble() * 100;
            double adicionalVip = random.nextDouble() * 100;
            double adicionalCamarote = random.nextDouble() * 100;

            normais[i] = new IngressoNormal(valorIngresso);
            vips[i] = new IngressoVip(valorIngresso, adicionalVip);
            camarotes[i] = new IngressoCamarote(valorIngresso, adicionalVip, adicionalCamarote);
        }

        for (int i = 0; i < 25; ++i) {
            System.out.printf("Ingresso Normal: ");
            normais[i].imprimirValorIngresso();
            System.out.printf("Ingresso VIP: ");
            vips[i].imprimirValorIngresso();
            System.out.printf("Ingresso Camarote: ");
            camarotes[i].imprimirValorIngresso();
            System.out.println();
        }
    }
}
