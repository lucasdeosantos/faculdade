package ingresso;

public class IngressoCamarote extends IngressoVip {
    private double adicionalCamarote;

    public IngressoCamarote() {}

    public IngressoCamarote(double valorIngresso, double adicionalVip, double adicionalCamarote) {
        super(valorIngresso, adicionalVip);
        this.setAdicionalCamarote(adicionalCamarote);
    }

    @Override
    public double getValorIngresso() {
        return super.getValorIngresso() + this.getAdicionalCamarote();
    }

    public double getAdicionalCamarote() {
        return this.adicionalCamarote;
    }

    public void setAdicionalCamarote(double adicionalCamarote) {
        if (adicionalCamarote >= 0.0)
            this.adicionalCamarote = adicionalCamarote;
    }
}