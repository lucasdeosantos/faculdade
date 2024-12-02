#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int n;
    int m;
    double *prices;
    double *costs;
    double *limits;
    double **compounds;
} Problem;

Problem *createProblem(int n, int m) {
    Problem *prob = (Problem *)malloc(sizeof(Problem));

    prob->n = n;
    prob->m = m;
    prob->prices = (double *)malloc(n * sizeof(double));
    prob->costs = (double *)malloc(m * sizeof(double));
    prob->limits = (double *)malloc(m * sizeof(double));

    prob->compounds = (double **)malloc(n * sizeof(double *));
    for (int i = 0; i < n; ++i) {
        prob->compounds[i] = (double *)malloc(m * sizeof(double));
    }

    return prob;
}

void freeProblem(Problem *prob) {
    free(prob->prices);
    free(prob->costs);
    free(prob->limits);
    for (int i = 0; i < prob->n; ++i) {
        free(prob->compounds[i]);
    }
    free(prob->compounds);
    free(prob);
}

Problem *readProblem() {
    int n, m;
    scanf("%d %d", &n, &m);

    Problem *prob = createProblem(n, m);

    for (int i = 0; i < n; ++i) {
        scanf("%lf", &prob->prices[i]);
    }

    for (int i = 0; i < m; ++i) {
        scanf("%lf %lf", &prob->costs[i], &prob->limits[i]);
    }

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            scanf("%lf", &prob->compounds[i][j]);
        }
    }

    return prob;
}

double calculateProfit(Problem *prob, int i) {
    double profit = 0;

    for (int j = 0; j < prob->m; j++)
        profit += prob->compounds[i][j] * prob->costs[j];
    
    return profit;
}

void printfLpSolve(Problem *prob) {
    //Imprime a função objetivo
    printf("max : %.1fx1", prob->prices[0] - calculateProfit(prob, 0));
    for (int i = 1; i < prob->n; ++i) {
        if (prob->prices[i] - calculateProfit(prob, i) > 0)
            printf(" + ");
        else if (prob->prices[i] - calculateProfit(prob, i) < 0)
            printf(" - ");
        else
            continue;
        printf("%.1fx%d", prob->prices[i] - calculateProfit(prob, i), i+1);
    }
    printf(";\n");
    
    //Imprime as restrições
    for (int j = 0; j < prob->m; ++j) {
        printf("%.1fx1", prob->compounds[0][j]);
        for(int i = 1; i < prob->n; ++i) {
            if (prob->compounds[i][j] > 0)
                printf(" + ");
            else if (prob->compounds[i][j] < 0)
                printf(" - ");
            else
                continue;
            printf("%.1fx%d", prob->compounds[i][j], i+1);
        }
        printf(" <= %.1f;\n", prob->limits[j]);
    }

    //Imprime a restrições de não negatividade
    for (int i = 0; i < prob->n; ++i) {
        printf("x%d >= 0;\n", i);
    }
}

int main() {
    Problem *prob = readProblem();
    printfLpSolve(prob);
    freeProblem(prob);

    return 0;
}
