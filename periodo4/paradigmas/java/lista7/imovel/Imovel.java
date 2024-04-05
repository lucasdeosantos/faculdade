package imovel;

public abstract class Imovel {
    protected Endereco endereco;
    protected double preco;

    public Imovel () {}

    public Imovel (Endereco endereco, double preco) {
        this.setEndereco(endereco);
        this.setPreco(preco);
    }

    public void setEndereco(Endereco endereco) {
        if (endereco != null)
            this.endereco = endereco;
    }

    public void setPreco(double preco) {
        if (preco >= 0.0)
            this.preco = preco;
    }

    public abstract double calcularValorImovel();
}
