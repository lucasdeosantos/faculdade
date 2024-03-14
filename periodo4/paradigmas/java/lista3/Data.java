public class Data {
    private int dia;
    private int mes;
    private int ano;

    public int getDia() {
        return this.dia;
    }

    public void setDia(int dia) {
        if (dia >= 1 && dia <= 31) {
            if (this.mes == 1 || this.mes == 3 || this.mes == 5 || this.mes == 7 || 
                this.mes == 8 || this.mes == 10 || this.mes == 12) {
                this.dia = dia;
            }
            else if ((this.mes == 4 || this.mes == 6 || this.mes == 9 || this.mes == 11) && dia <= 30) {
                this.dia = dia;
            }
            else if (this.mes == 2) {
                if (bisexto(this.ano) == true && dia <= 29)
                    this.dia = dia;
                else if (bisexto(this.ano) == false && dia <= 28)
                    this.dia = dia;
            }
        }
    }
    
    private static boolean bisexto(int ano) {
        return (ano % 4 == 0 && ano % 100 != 0) || (ano % 400 == 0);
    }

    public int getMes() {
        return this.mes;
    }

    public void setMes(int mes) {
        if (mes >= 1 && mes <= 12)
            this.mes = mes;
    }

    public int getAno() {
        return this.ano;
    }

    public void setAno(int ano) {
        if (ano >= 1)
            this.ano = ano;
    }
}
