public class Data {
    private int dia;
    private int mes;
    private int ano;

    public Data(int dia, int mes, int ano) {
        this.setDia(dia);
        this.setMes(mes);
        this.setAno(ano);
    }

    public int getDia() {
        return this.dia;
    }

    public int getMes() {
        return this.mes;
    }

    public int getAno() {
        return this.ano;
    }

    public void setDia(int dia) {
        if (dia >= 1 && dia <= 31) {
            if (this.mes == 2 && dia > 29) {
                throw new IllegalArgumentException("fevereiro tem no máximo 29 dias.");
            }
            else if (this.mes == 2 && dia == 29 && this.ano != 0 && !bisexto(this.ano)) {
                throw new IllegalArgumentException(this.ano + " não é um ano bisexto.");
            }
            else if ((this.mes == 4 || this.mes == 6 || this.mes == 9 || this.mes == 11) && dia > 30) {
                throw new IllegalArgumentException(this.mes + " tem no máximo 30 dias.");
            }
            else {
                this.dia = dia;
            }
        }
        else 
            throw new IllegalArgumentException("um dia deve estar no intervalo [1, 31].");
    }

    public void setMes(int mes) {
        if (mes >= 1 && mes <= 12) {
            if (mes == 2 && this.dia > 29 ) {
                throw new IllegalArgumentException("fevereiro tem no máximo 29 dias.");
            }
            else if (mes == 2 && this.dia == 29 && this.ano != 0 && !bisexto(this.ano)) {
                throw new IllegalArgumentException(this.ano + " não é um ano bisexto.");
            }
            else if ((mes == 4 || mes == 6 || mes == 9 || mes == 11) && this.dia > 30) {
                throw new IllegalArgumentException(mes + " tem no máximo 30 dias.");
            }
            else {
                this.mes = mes;
            }
        } 
        else {
            throw new IllegalArgumentException("um mês deve estar no intervalo [1, 12].");
        }
    }

    public void setAno(int ano) {
        if (ano >= 1) {
            if (this.mes != 2 || this.dia <= 28 || this.dia == 29 && bisexto(ano)) {
                this.ano = ano;
            }
            else {
                throw new IllegalArgumentException("o ano deveria ser bisexto.");
            }
        }
        else { 
            throw new IllegalArgumentException("um ano deve estar no intervalo [1, infinito].");
        }
    }

    private static boolean bisexto(int ano) {
        return (ano % 4 == 0 && ano % 100 != 0) || (ano % 400 == 0);
    }

    public boolean anterior(int dia, int mes, int ano) {
        if (this.ano > ano)
            return true;
        else if (this.ano == ano && this.mes > mes)
            return true;
        else if (this.ano == ano && this.mes == mes && this.dia > dia)
            return true;
        else 
            return false;
    }

    public boolean posterior(int dia, int mes, int ano) {
        if (this.ano < ano)
            return true;
        else if (this.ano == ano && this.mes < mes)
            return true;
        else if (this.ano == ano && this.mes == mes && this.dia < dia)
            return true;
        else 
            return false;
    }

    public boolean equals(int dia, int mes, int ano) {
        if (this.dia == dia && this.mes == mes && this.ano == ano)
            return true;

        return false;
    }

    public String toString() {
        if (this.dia == 0 || this.mes == 0 || this.ano == 0)
            return "data inválida.";

        return dia + "/" + mes + "/" + ano;
    }
}
