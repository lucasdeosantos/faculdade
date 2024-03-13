public class Telefone {
    private int codigoPais;
    private int codigoArea;
    private long numero;
    
    public void cadastrar(int codigoPais, int codigoArea, long numero) {
        this.codigoPais = codigoPais;
        this.codigoArea = codigoArea;
        this.numero = numero;
    }

    public void exibir() {
        System.out.printf("+%d(%d)%d\n", this.codigoPais, this.codigoArea, this.numero);
    }
}