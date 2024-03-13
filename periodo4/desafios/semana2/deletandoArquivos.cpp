#include <bits/stdc++.h>
using namespace std;
using ll = long long;

int prioridade(const string formato) {
    if (formato == "TMP")
        return 2;
    else if (formato == "OGG")
        return 1;
    else
        return 0;
}
 
int main() {
    cin.tie();
    ios_base::sync_with_stdio(0);

    int n, k;
    cin >> n >> k;
    vector<tuple<ll, int, ll>> arquivos(n);

    for (int i = 0; i < n; i++) {
        ll utilizacoes, tamanho;
        string formato;

        cin >> utilizacoes >> formato >> tamanho;
        auto arq = make_tuple(utilizacoes, prioridade(formato), tamanho);
        arquivos[i] = arq;
    }

    sort(arquivos.begin(), arquivos.end(), [](const auto& a, const auto& b) {
        if (get<0>(a) != get<0>(b))
            return get<0>(a) < get<0>(b);
        else
            return (get<1>(a) > get<1>(b)) || (get<1>(a) == get<1>(b) && get<2>(a) > get<2>(b));
    });

    ll espacoLiberado = 0;
    for (int i = 0; i < k; i++)
        espacoLiberado += get<2>(arquivos[i]);

    cout << espacoLiberado << "\n";
}
