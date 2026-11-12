public class Ecomper {
    private static int idCounter = 1;

    private int id;
    private String nome;
    private String cpf;
    private String email;
    private String cargo;

    public Ecomper(String nome, String cpf, String email, String cargo) {
        this.id = idCounter++;
        this.nome = nome;
        this.cpf = cpf;
        this.email = email;
        this.cargo = cargo;
    }

    protected Ecomper(int id, String nome, String cpf, String email, String cargo) {
        this.id = id;
        this.nome = nome;
        this.cpf = cpf;
        this.email = email;
        this.cargo = cargo;
    }

    public int getId() { return id; }

    public String getNome() { return nome; }
    public String getCpf() { return cpf; }
    public String getEmail() { return email; }
    public String getCargo() { return cargo; }

    public void setNome(String nome) { this.nome = nome; }
    public void setCpf(String cpf) { this.cpf = cpf; }
    public void setEmail(String email) { this.email = email; }
    public void setCargo(String cargo) { this.cargo = cargo; }

    @Override
    public String toString() {
        return "Id: " + getId() + "\n"
            + "Nome: " + getNome() + "\n"
            + "CPF: " + getCpf() + "\n"
            + "Email: " + getEmail() + "\n"
            + "Cargo: " + getCargo();
    }
}
