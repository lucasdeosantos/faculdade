#include <bits/stdc++.h>
using namespace std;

int indice (char c) {
    return int(c) - 65;
}

char caracter (int n) {
    return char(n + 65);
}

int main () {
    cin.tie();
    ios_base::sync_with_stdio(0);

    string text;
    cin >> text;
    vector<int> caracteres(26, 0);

    for (char c : text)
        caracteres[indice(c)]++;

    string resultado;
    int imparCount = 0;
    int nImpar;
    char imparCaracter;

    for (int i = 0; i < 26; i++) {
        if (caracteres[i] % 2 == 0 && caracteres[i] != 0) {
            if (resultado.empty()) {
                resultado += string(caracteres[i], caracter(i));
            }
            else {
                int meio = resultado.size() / 2;
                resultado.insert(meio, string(caracteres[i], caracter(i)));
            }
        }
        else if (caracteres[i] % 2 == 1){
            imparCaracter = caracter(i);
            nImpar = caracteres[i];
            imparCount++;
        }
    }

    if (imparCount > 1) {
        cout << "NO SOLUTION\n";
    }
    else {
        if (imparCount == 1) {
            int meio = resultado.size() / 2;
            resultado.insert(meio, string(nImpar, imparCaracter));
        }
        cout << resultado;
    }
}