package cartao;

public abstract class CartaoWeb {
    protected String destinatario;

    public CartaoWeb() {}

    public CartaoWeb(String destinatario) {
        this.destinatario = destinatario;
    }

    public abstract void retornarMensagem(String remetente);
}
