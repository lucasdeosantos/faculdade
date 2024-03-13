#include <bits/stdc++.h>
using namespace std;

int calcularResultado(int a, int b, int c, char op1, char op2) {
    if (op1 == '+') {
        if (op2 == '+') {
            return a + b + c;
        } else if (op2 == '-') {
            return a + b - c;
        } else if (op2 == '*') {
            return a + b * c;
        }
    } else if (op1 == '-') {
        if (op2 == '+') {
            return a - b + c;
        } else if (op2 == '-') {
            return a - b - c;
        } else if (op2 == '*') {
            return a - b * c;
        }
    } else if (op1 == '*') {
        if (op2 == '+') {
            return a * b + c;
        } else if (op2 == '-') {
            return a * b - c;
        } else if (op2 == '*') {
            return a * b * c;
        }
    }

    return INT_MAX;
}

int main() {
    cin.tie();
    ios_base::sync_with_stdio(0);

    int a, b, c;
    cin >> a >> b >> c;

    char operadores[] = {'+', '-', '*'};
    int resultadoMinimo = INT_MAX;

     for (char op1 : operadores) {
        for (char op2 : operadores) {
            int resultadoAtual = calcularResultado(a, b, c, op1, op2);
            resultadoMinimo = min(resultadoMinimo, resultadoAtual);
        }
    }
    cout << resultadoMinimo << "\n";
}