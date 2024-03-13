#include <bits/stdc++.h>
using namespace std;
using ll = long long;

ll minMovimentos (const vector<ll>& vetor) {
    ll movimentos = 0;
    ll atual = vetor[0];

    for (size_t i = 1; i < vetor.size(); ++i) {
        if (vetor[i] < atual)
            movimentos += atual - vetor[i];
        else
            atual = vetor[i];
    }

    return movimentos;
}

int main() {
    cin.tie();
    ios_base::sync_with_stdio(0);

    ll n;
    cin >> n;
    vector<ll> vetor(n);

    for (size_t i = 0; i < n; i++)
        cin >> vetor[i];

    ll resultado = minMovimentos(vetor);
    cout << resultado << "\n";
}