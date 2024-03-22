package lampada;

import java.util.Scanner;

public class Main {
    public static void main (String[] args) {
        Scanner input = new Scanner(System.in);
        LampadaFluorecente fluorecente = new LampadaFluorecente(true, 12);
        LampadaLed led = new LampadaLed(false);

        System.out.println("Escolha um tipo de lampada:");
        System.out.println("1: lampada fluroescente.");
        System.out.println("2: lampada led.");
        int tipo = input.nextInt();

        if (tipo == 1 && fluorecente.isLigada() == true) {
            System.out.println("A lampada fluorescente esta ligada.");
            trocarEstado(fluorecente, true);
        }
        else if (tipo == 1 && fluorecente.isLigada() == false) {
            System.out.println("A lampada fluorescente esta desligada.");
            trocarEstado(fluorecente, false);
        }
        else if (tipo == 2 && led.isLigada() == true) {
            System.out.println("A lampada led esta ligada.");
            trocarEstado(led, true);
        }
        else {
            System.out.println("A lampada led esta desligada.");
            trocarEstado(led, false);
        }
    }

    public static void trocarEstado(Lampada lampada, boolean ligada) {
        Scanner input = new Scanner(System.in);

        System.out.println("Voce deseja alterar o estado da lampada? [yes] [no]");

        String estado = input.nextLine();

        if (estado == "yes") {
            if (ligada == true)
                lampada.desligarLampada();
            else
                lampada.ligarLampada();
        }
        else if (estado == "no") {
            if (ligada == true)
                lampada.desligarLampada();
            else
                lampada.ligarLampada();
        }
    }
}
