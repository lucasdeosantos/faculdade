package cartao;

public class CartaoNatal extends CartaoWeb {
    public CartaoNatal() {}

    public CartaoNatal (String destinatario) {
        super(destinatario);
    }

    @Override
    public void retornarMensagem(String rementente) {
        System.out.println("\"Querida " + this.destinatario + ",");
        System.out.println("Feliz Natal!");
        System.out.println("Que as luzes cintilantes do Natal encham seu coração de paz,");
        System.out.println("e que o ano novo traga consigo oportunidades e realizações.");
        System.out.println("De todo o coração,");
        System.out.println(rementente + "\"");
    }
}
