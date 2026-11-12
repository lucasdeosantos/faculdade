public class Cliente {
    private static int idCounter = 1;

    private int id;
    private String nome;
    private String cpfCnpj;
    private String email;
    private String telefone;
    private String contato;

    public Cliente(String nome, String cpfCnpj, String email, String telefone, String contato) {
        this.id = idCounter++;
        this.nome = nome;
        this.email = email;
        this.cpfCnpj = cpfCnpj;
        this.telefone = telefone;
        this.contato = contato;
    }

    public int getId() { return id; }

    public String getNome() { return nome; }
    public String getEmail() { return email; }
    public String getCpfCnpj() { return cpfCnpj; }
    public String getTelefone() { return telefone; }
    public String getContato() { return contato; } 

    public void setNome(String nome) { this.nome = nome; }
    public void setEmail(String email) { this.email = email; }
    public void setCpfCnpj(String cpfCnpj) { this.cpfCnpj = cpfCnpj; }
    public void setTelefone(String telefone) { this.telefone = telefone;}
    public void setContato(String contato) { this.contato = contato; }

    @Override
    public String toString() {
        return "Id: " + id + "\n"
            + "Nome: " + nome + "\n"
            + "Email: " + email + "\n"
            + "CPF/CNPJ: " + cpfCnpj + "\n"
            + "Telefone: " + telefone + "\n"
            + "Contato: " + contato;
    }
}
