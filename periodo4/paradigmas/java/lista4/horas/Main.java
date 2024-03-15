public class Main {
    public static void main (String[] args) {
        Time primeira = new Time();
        Time segunda = new Time();

        primeira.setTime(8, 35);
        segunda.setTime(13, 19);

        System.out.println(primeira.toString());
        System.out.println(segunda.toString());

        if (primeira.anterior(segunda.getHora(), segunda.getMinuto()))
            System.out.println("anterior");
        else  if (primeira.posterior(segunda.getHora(), segunda.getMinuto()))
            System.out.println("posterior");
        else
            System.out.println("equals");
    }
}

