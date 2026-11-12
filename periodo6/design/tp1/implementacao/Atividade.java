import java.util.ArrayList;
import java.util.List;

public class Atividade {
    private String nome;
    private String duracao;
    private String local;
    private String objetivo;
    private String resumo;
    private List<Participante> participantes;

    public Atividade(String nome, String duracao, String local, String objetivo, String resumo) {
        this.nome = nome;
        this.duracao = duracao;
        this.local = local;
        this.objetivo = objetivo;
        this.resumo = resumo;
        this.participantes = new ArrayList<>();
    }

    public String getNome () { return nome; }
    public String getDuracao () { return duracao; }
    public String getLocal () { return local; }
    public String getObjetivo () { return objetivo; }
    public String getResumo () { return resumo; }

    public void setNome (String nome) { this.nome = nome; }
    public void setDuracao (String duracao) { this.duracao = duracao; }
    public void setLocal (String local) { this.local = local; }
    public void setObjetivo (String objetivo) { this.objetivo = objetivo; }
    public void setResumo (String resumo) { this.resumo = resumo; }

    public void adicionarParticipante(Participante p) {
        this.participantes.add(p);
    }

    @Override
    public String toString() {
        return "Nome: " + nome + "\n"
            + "Objetivo: " + objetivo + "\n"
            + "Duracao: " + duracao + "\n"
            + "Local: " + local + "\n"
            + "Resumo: " + resumo + "\n"
            + "Participantes: " + participantes;
    }
}
