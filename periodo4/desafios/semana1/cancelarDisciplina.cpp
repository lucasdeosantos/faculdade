#include <bits/stdc++.h>
using namespace std;

int main () {
    cin.tie();
    ios_base::sync_with_stdio(0);

    string disciplina;
    int horas;
    int minutos;

    while (cin >> disciplina) {
        cin  >> horas >> minutos;
        if (horas > 10 || (horas == 10 && minutos > 0)) {
            cout << "Abel deve cancelar " << disciplina << "\n";
        }
        else {
            cout << "Abel deve cursar " << disciplina << "\n";
        }
    }  
}