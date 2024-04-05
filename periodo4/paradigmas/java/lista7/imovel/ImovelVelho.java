package imovel;

public class ImovelVelho extends Imovel {
    public ImovelVelho () {}

    public ImovelVelho(Endereco endereco, double preco) {
        super(endereco, preco);
    }

    public double calcularValorImovel() {
        return 1;
    }
}
