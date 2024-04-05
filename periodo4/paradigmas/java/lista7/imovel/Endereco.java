package imovel;

public class Endereco {
    private String rua;
    private String bairro;
    private String cidade;
    private String estado;
    private int numero = 0;

    public Endereco () {}

    public Endereco (String rua, String bairro, String cidade, String estado, int numero) {
        this.setRua(rua);
        this.setBairro(bairro);
        this.setCidade(cidade);
        this.setEstado(estado);
        this.setNumero(numero);
    }

    public String getRua() {
        return this.rua;
    }

    public String getBairro() {
        return this.bairro;
    }

    public String getCidade() {
        return this.cidade;
    }

    public String getEstado() {
        return this.estado;
    }

    public int getNumero() {
        return this.numero;
    }

    public void setRua(String rua) {
        if (rua != null)
            this.rua = rua;
    }

    public void setBairro(String bairro) {
        if (bairro != null)
            this.bairro = bairro;
    }

    public void setCidade(String cidade) {
        if (cidade != null)
            this.cidade = cidade;
    }

    public void setEstado(String estado) {
        if (estado != null)
            this.estado = estado;
    }
    
    public void setNumero(int numero) {
        if (numero > 0)
            this.numero = numero;
    }

    public String toString() {
        if(this.rua == null || this.bairro == null || this.cidade == null || this.estado == null || this.numero == 0)
            return "endereço inválido.";

        return this.rua + ", " + numero + ", " + bairro + ", " + cidade + " - " + estado + "."; 
    }
}
