import java.util.Scanner;

public class Turma {
    public static void main(String[] args) {
        Scanner input = new Scanner(System.in);

        int numEstudantes = input.nextInt();
        input.nextLine();
        Estudante[] turma = new Estudante[numEstudantes];

        int notasTotais = 0;

        for (int i = 0; i < numEstudantes; i++) {
            Estudante estudante = new Estudante();
            estudante.nomear(input.nextLine());
            int nota = input.nextInt();
            input.nextLine();
            estudante.avaliar(nota);
            notasTotais += nota;

            turma[i] = estudante;
        }

        System.out.println("A media da turma foi: " + (notasTotais / numEstudantes));

        int aprovados, exameFinal, reprovados;
        aprovados = exameFinal = reprovados = 0;
        for (Estudante estudante : turma) {
            if (estudante.retornarNota() >= 70)
                aprovados++;
            else if (estudante.retornarNota() >=40)
                exameFinal++;
            else
                reprovados++;
        }

        System.out.println("A quantidade de alunos aprovado: " + aprovados);
        System.out.println("A quantidade de alunos em exame final: " + exameFinal);
        System.out.println("A quantidade de alunos reprovados: " + reprovados);
    }
}
