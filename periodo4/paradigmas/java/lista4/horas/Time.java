public class Time {
    private int h;
    private int m;

    public int getHora() {
        return this.h;
    }

    public int getMinuto() {
        return this.m;
    }

    public void setHora(int h) {
        if (h > 1 && h <= 24)
            this.h = h;
        else
            throw new IllegalArgumentException("uma hora deve estar no intervalo (1, 24].");
    }

    public void setMinuto(int m) {
        if (m >= 0 && m <= 60)
            this.m = m;
        else
            throw new IllegalArgumentException("um minuto deve estar no intervalo [0, 60].");
    }

    public void setTime(int h, int m) {
        setHora(h);
        setMinuto(m);
    }

    public boolean anterior(int h, int m) {
        if (this.h > h)
            return true;
        else if (this.h == h && this.m > m)
            return true;
        else 
            return false;
    }

    public boolean posterior(int h, int m) {
        if (this.h < h)
            return true;
        else if (this.h == h && this.m < m)
            return true;
        else 
            return false;
    }

    public boolean equals(int h, int m) {
        if (this.h == h && this.m == m)
            return true;

        return false;
    }

    public String toString() {
        if (this.h == 0 || this.m == 0)
            return "hórario inválido.";

        return this.h + ":" + this.m;
    }
}
