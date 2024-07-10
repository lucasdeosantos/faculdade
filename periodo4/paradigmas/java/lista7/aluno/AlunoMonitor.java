package aluno;

public class AlunoMonitor {
    private String nome;
    private long matricula;
    private String codigoDisciplina;

    public AlunoMonitor() {}

    public AlunoMonitor(String nome, long matricula, String codigoDisciplina) {
        this.setNome(nome);
        this.setMatricula(matricula);
        this.setCodigoDisciplina(codigoDisciplina);
    }

    public String getNome() {
        return this.nome;
    }

    public long getMatricula() {
        return this.matricula;
    }

    public String getCodigoDisciplina() {
        return this.codigoDisciplina;
    }

    public void setNome(String nome) {
        if (nome != null)
            this.nome = nome;
    }

    public void setMatricula(long matricula) {
        if (matricula > 0)
            this.matricula = matricula;
    }

    public void setCodigoDisciplina(String codigoDisciplina) {
        if (codigoDisciplina != null)
            this.codigoDisciplina = codigoDisciplina;
    }
}
