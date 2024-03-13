#include <bits/stdc++.h>
using namespace std;
using ll = long long;

int main() {
    cin.tie();
    ios_base::sync_with_stdio(0);

    list<ll> chifre;
    auto iterador = chifre.begin();
    int n;
    cin >> n;
    for (int i = 0; i < n; i++) {
        char operacao;
        cin >> operacao;

        switch (operacao) {
            case 'm':
                int d;
                cin >> d;
                advance(iterador, d);
                break;

            case 'i':
                int q;
                ll t;
                cin >> q >> t;
                iterador = chifre.insert(iterador, q, t);
                break;

            case 'd':
                iterador = chifre.erase(iterador);
                break;

            case 'q':
                cout << *iterador << "\n";
                break;
        }
    }
}
