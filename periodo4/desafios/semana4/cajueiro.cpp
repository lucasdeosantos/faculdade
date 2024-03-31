#include <bits/stdc++.h>
using namespace std;

int maxCajus(vector<int>& cajus) {
    int n = cajus.size();
    vector<int> aux(n + 1, 0);
    aux[0] = 0;

    for (int i = 1; i <= n; ++i) {
        aux[i] = max(aux[i - 1], cajus[i - 1] + (i >= 2 ? aux[i - 2] : 0));

        if (i >= 3) {
            aux[i] = max(aux[i], cajus[i - 1] + cajus[i - 2] + aux[i - 3]);
        }
    }

    return aux[n];
}

int main() {
    int n;
    cin >> n;
    vector<int> cajus(n);
    for (int i = 0; i < n; ++i)
        cin >> cajus[i];

    cout << maxCajus(cajus) << "\n";
}
