package imovel;

public class ImovelNovo extends Imovel {
    public ImovelNovo () {}

    public ImovelNovo(Endereco endereco, double preco) {
        super(endereco, preco);
    }

    public double calcularValorImovel() {
        return preco;
    }
}
