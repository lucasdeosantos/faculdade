import java.util.Scanner;

public class Notas {
    public static void main (String[] args) {
        Scanner input = new Scanner(System.in);

        int avaliacoes = 4, alunos = 30;
        int[][] notas = new int[alunos][avaliacoes];

        for (int i = 0; i < alunos; i++) {
            System.out.printf("Digite as notas do aluno %d:\n", i + 1);
            for (int j = 0; j < avaliacoes; j++) {
                System.out.printf("P%d: ", j + 1);
                notas[i][j] = input.nextInt();
            }
        }

        int maiorNotaTurma = 0, maiorNotaAluno = 0;

        for (int i = 0; i < alunos; i++) {
            for (int j = 0; j < avaliacoes; j++) {
                maiorNotaAluno = max(maiorNotaAluno, notas[i][j]);
            }
            System.out.printf("A maior nota do aluno %d foi %d.\n", i + 1, maiorNotaAluno);
            maiorNotaTurma = max(maiorNotaTurma, maiorNotaAluno);
            maiorNotaAluno = 0;
        }
        System.out.printf("A maior nota da turma foi %d.\n", maiorNotaTurma);
    }

    public static int max(int a, int b) {
        return a > b ? a : b;
    }
}
