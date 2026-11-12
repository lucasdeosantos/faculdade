public class ItemFiscal {
    private String data;
    private float valor;
    private String tipo;

    public ItemFiscal(String data, float valor, String tipo) {
        this.data = data;
        this.valor = valor;
        this.tipo = tipo;
    }

    public String getData() { return data; }
    public float getValor() { return valor; }
    public String getTipo() { return tipo; }

    public void setData(String novaData) { this.data = novaData; }
    public void setValor(float novoValor) { this.valor = novoValor; }
    public void setTipo(String novoTipo) { this.tipo = novoTipo; }

    @Override
    public String toString() {
        return "Tipo: " + tipo + "\n"
            + "Data: " + data + "\n"
            + "Valor: " + valor;
    }
}
