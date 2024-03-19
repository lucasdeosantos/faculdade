#include <bits/stdc++.h>
using namespace std;

int main() {
    cin.tie();
    ios_base::sync_with_stdio(0);
    
    int n;
    cin >> n;

    vector<int> equacao(n);

    for (int i = 0; i < n; ++i)
        cin >> equacao[i];
        
    unordered_map<int, int> contador;

    for (int a : equacao)
        for (int b : equacao)
            for (int c : equacao)
                contador[a * b + c]++;

    int satisfaz = 0;

    for (int d : equacao)
        if (d)
            for (int e : equacao)
                for (int f : equacao)
                    satisfaz += contador[(e + f) * d];

    cout << satisfaz << "\n";
    return 0;
}