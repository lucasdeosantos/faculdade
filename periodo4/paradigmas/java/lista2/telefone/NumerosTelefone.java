public class NumerosTelefone {
    public static void main (String[] args) {
        Telefone[] numerosTelefone = new Telefone[5];

        numerosTelefone[0] = new Telefone();
        numerosTelefone[0].cadastrar(55, 41, 987654321);
        numerosTelefone[0].exibir();
    }
}