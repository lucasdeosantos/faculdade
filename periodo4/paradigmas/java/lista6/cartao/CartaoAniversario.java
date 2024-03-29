package cartao;

public class CartaoAniversario extends CartaoWeb {
    public CartaoAniversario() {}

    public CartaoAniversario (String destinatario) {
        super(destinatario);
    }
    
    @Override
    public void retornarMensagem(String rementente) {
        System.out.println("\"Querida " + this.destinatario + ",");
        System.out.println("Feliz Aniversario!");
        System.out.println("Hoje é um dia especial, pois celebramos você!");
        System.out.println("Que você seja cercado por amor e carinho todos os dias da sua vida.");
        System.out.println("De todo o coração,");
        System.out.println(rementente + "\"");
    }
}
