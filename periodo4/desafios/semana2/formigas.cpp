#include <bits/stdc++.h>
using namespace std;
using ll = long long;

#define menor first
#define maior second

int main() {
    cin.tie();
    ios_base::sync_with_stdio(0);

    int t;
    cin >> t;

    for (int i = 0; i < t; i++) {
        int formigas;
        ll tamanhoPoste;
        pair <ll, ll> resultado, atual;
        cin >> tamanhoPoste >> formigas;
        
        for (int i = 0; i < formigas; i++) {
            ll posicao;
            cin >> posicao;

            ll tamanhoTopo = tamanhoPoste - posicao;
            ll tamanhoBase = posicao;

            atual = (tamanhoTopo > tamanhoBase) ? make_pair(tamanhoBase, tamanhoTopo) : make_pair(tamanhoTopo, tamanhoBase);

            if (atual.menor > resultado.menor)
                resultado.menor = atual.menor;

            if (atual.maior > resultado.maior)
                resultado.maior = atual.maior;
        }
        cout << resultado.menor << " " << resultado.maior << "\n";
    }
}
