public class Estudante {
    private String nome;
    private int nota;
    
    public void nomear(String nome) {
        this.nome = nome;
    }

    public void avaliar(int nota) {
        this.nota = nota;
    }

    public void exibirNome() {
        System.out.println(this.nome);
    }

    public void exibirNota() {
        System.out.println("Nota final: " + this.nota);
    }

    public int retornarNota() {
        return this.nota;
    }
}
