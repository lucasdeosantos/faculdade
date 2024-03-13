#include <bits/stdc++.h>
using namespace std;

int main() {
    cin.tie();
    ios_base::sync_with_stdio(0);

    int n;
    cin >> n;

    vector<int> numeros(n);
    for (int i = 0; i < n; ++i)
        cin >> numeros[i];

    vector<int> ordenados(n);
    for (int i = 0; i < n; ++i)
        ordenados[numeros[i] - 1] = i + 1;

    int coletor = 1;

    for (int i = 0; i < (n-1); ++i)
        if (ordenados[i] > ordenados[i+1])
            coletor++;

    cout << coletor << "\n";
}
