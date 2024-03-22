package ingresso;

public class IngressoVip extends Ingresso {
    protected double adicionalVip;

    public IngressoVip () {}

    public IngressoVip (double valorIngresso, double adicionalVip) {
        super(valorIngresso);
        this.setAdicionalVip(adicionalVip);;
    }
    
    @Override
    public double getValorIngresso() {
        return super.getValorIngresso() + this.getAdicionalVip();
    }

    public double getAdicionalVip() {
        return this.adicionalVip;
    }

    public void setAdicionalVip(double adicionalVip) {
        if (adicionalVip >= 0.0)
            this.adicionalVip = adicionalVip;
    }
}
