package lampada;

public class LampadaFluorecente extends Lampada {
    private int comprimento;

    public LampadaFluorecente () {}

    public LampadaFluorecente (boolean ligada, int comprimento) {
        super(ligada);
        this.setComprimento(comprimento);;
    }

    public int getComprimento() {
        return this.comprimento;
    }

    public void setComprimento (int comprimento) {
        if (comprimento > 0)
            this.comprimento = comprimento;
    }
}
