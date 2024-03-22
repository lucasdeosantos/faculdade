package ingresso;

import java.text.NumberFormat;
import java.util.Locale;

public class Ingresso {
    protected double valorIngresso;
    
    public Ingresso() {}

    public Ingresso (double valorIngresso) {
        this.setValorIngresso(valorIngresso);
    }

    public double getValorIngresso() {
        return this.valorIngresso;
    }

    public void setValorIngresso(double valorIngresso) {
        if (valorIngresso >= 0.0)
            this.valorIngresso = valorIngresso;
    }

    public void imprimirValorIngresso() {
        Locale localeBrasil = Locale.forLanguageTag("pt-BR");
        NumberFormat valor = NumberFormat.getCurrencyInstance(localeBrasil);
        System.out.println(valor.format(this.getValorIngresso()));
    }
}
