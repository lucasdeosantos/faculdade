public class Contador {
    private long contador;
    private long passo = 1;

    public long getContador() {
        return this.contador;
    }

    public long getPasso() {
        return this.passo; 
    }

    public void setContador(long contador) {
        if (contador >= 0)
            this.contador = contador;
        else
            throw new IllegalArgumentException("o contador não pode ter um valor negativo.");
    }

    public void setPasso(long passo) {
        this.passo = passo;
    }

    public void zerarContador() {
        this.contador = 0;
    }

    public void incrementaContador() {
        this.contador += this.passo;
    }

    public void incrementaContador(long valor) {
        this.contador += valor;
    }

    public void decrementaContador() {
        if (this.contador >= this.passo)
            this.contador -= this.passo;
        else
            this.contador = 0;
    }

    public void decrementaContador(long valor) {
        if (this.contador >= valor)
            this.contador -= valor;
        else
            this.contador = 0;
    }
}