#include <bits/stdc++.h>
using namespace std;

bool parceiros(const string& x, const string& y) {
    int ultimoDigitoX = x.back() - '0';
    int ultimoDigitoY = y.back() - '0';

    return (ultimoDigitoX + ultimoDigitoY) % 2 == 0;
}

int main () {
    cin.tie();
    ios_base::sync_with_stdio(0);
    int n;
    cin >> n;
    for (int i = 0; i < n; ++i) {
        string x, y;
        cin >> x >> y;
        if (parceiros(x, y))
            cout << "PARCEIROS\n";
        else
            cout << "NAO PARCEIROS\n";
    }
}