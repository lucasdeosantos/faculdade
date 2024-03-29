package cartao;

public class CartaoDiaDosNamorados extends CartaoWeb {
    public CartaoDiaDosNamorados() {}

    public CartaoDiaDosNamorados (String destinatario) {
        super(destinatario);
    }

    @Override
    public void retornarMensagem(String rementente) {
        System.out.println("\"Querida " + this.destinatario + ",");
        System.out.println("Feliz dia dos Namorados!");
        System.out.println("Espero que esse tenha sido o único do dia dos namorados que tenha");
        System.out.println("ganhado nessa data!");
        System.out.println("De todo o coração,");
        System.out.println(rementente + "\"");
    }
}

