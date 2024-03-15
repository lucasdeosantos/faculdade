import java.util.Scanner;
import java.time.LocalDate;

public class Programadoras {
    public static void main (String[] args) {
        Pessoa adaLovelace = new Pessoa();
        Pessoa graceHopper = new Pessoa();

        adaLovelace.nomear("Augusta Ada Byron King");
        adaLovelace.ajustarDataDeNascimento(10, 12, 1815);

        graceHopper.nomear("Grace Murray Hopper");
        graceHopper.ajustarDataDeNascimento(9,12,1906);

        LocalDate dataAtual = LocalDate.now();
        int diaAtual = dataAtual.getDayOfMonth();
        int mesAtual = dataAtual.getMonthValue();
        int anoAtual = dataAtual.getYear();

        graceHopper.calcularIdade(diaAtual, mesAtual, anoAtual);
        adaLovelace.calcularIdade(diaAtual, mesAtual, anoAtual);

        System.out.println(adaLovelace.retornarNome() + " teria " + adaLovelace.retornarIdade() + " anos.");
        System.out.println(graceHopper.retornarNome() + " teria " + graceHopper.retornarIdade() + " anos.");
    }
}
