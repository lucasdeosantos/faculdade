#include <bits/stdc++.h>
using namespace std;
#define optimize() ios_base::sync_with_stdio(0); cin.tie();
typedef long long ll;

void solve() {
    int N;
    cin >> N;
    
    vector<ll> cubes(N);
    for (int i = 0; i < N; ++i)
        cin >> cubes[i];

    multiset<int> towers;

    for (int i = 0; i < N; ++i) {
        auto it = towers.upper_bound(cubes[i]);
        if (it != towers.end())
            towers.erase(it);

        towers.insert(cubes[i]);
    }

    cout << towers.size() << "\n";
}

int main() {
    optimize();

    int t = 1;
    while(t--)
        solve();
}
