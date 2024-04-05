#include <bits/stdc++.h>
using namespace std;
#define optimize() ios_base::sync_with_stdio(0); cin.tie();

void solve() {
    int N, harvested = 0;
    cin >> N;
    for (int i = 0; i < N; ++i) {
        int A;
        cin >> A;
        if (A > 10)
            harvested += A - 10;
    }

    cout << harvested << "\n";
}

int main() {
    optimize();

    int t = 1;
    while(t--)
        solve();
}
