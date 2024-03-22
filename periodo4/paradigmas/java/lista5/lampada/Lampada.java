package lampada;

public class Lampada {
    protected boolean ligada;

    public Lampada() {}

    public Lampada(boolean ligada) {
        this.ligada = ligada;
    }

    public boolean isLigada() {
        return this.ligada;
    }

    public void setLigada(boolean ligada) {
        this.ligada = ligada;
    }

    public void ligarLampada() {
        this.ligada = true;
    }

    public void desligarLampada() {
        this.ligada = false;
    }
}