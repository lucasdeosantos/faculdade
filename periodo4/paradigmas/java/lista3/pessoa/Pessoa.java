import java.time.LocalDate;
import java.time.Period;

public class Pessoa {
    private String nome;
    private int idade;
    private int diaNascimento;
    private int mesNascimento;
    private int anoNascimento;

    public void nomear(String nome) {
        this.nome = nome;
    }

    public void ajustarDataDeNascimento(int dia, int mes, int ano) {
        this.diaNascimento = dia;
        this.mesNascimento = mes;
        this.anoNascimento = ano;
    }

    public String retornarNome() {
        return this.nome;
    }

    public int retornarIdade() {
        return this.idade;
    }

    public void calcularIdade(int diaAtual, int mesAtual, int anoAtual) {
        LocalDate dataNascimento = LocalDate.of(this.anoNascimento, this.mesNascimento, this.diaNascimento);
        LocalDate dataAtual = LocalDate.of(anoAtual, mesAtual, diaAtual);
        Period periodo = Period.between(dataNascimento, dataAtual);

        this.idade = periodo.getYears();
    }
}
