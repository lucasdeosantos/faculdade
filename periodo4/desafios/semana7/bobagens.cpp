#include <bits/stdc++.h>
using namespace std;
#define optimize() ios_base::sync_with_stdio(0); cin.tie();
#define x first
#define y second
typedef long long ll;
typedef vector<ll> vl;

map<int, int> factorize(int n) {
    map<int, int> factors;

    for (int i = 2; i <= sqrt(n); ++i)
        while (n % i == 0) {
            factors[i]++;
            n /= i;
        }

    if (n > 1) factors[n]++;

    return factors;
}

void solve() {
    int K;
    cin >> K;

    ll N = 1;
    for (int i = 0; i < K; ++i) {
        int P, E;
        cin >> P >> E;
        while (E--) N *= P;
    }

    N -= 1;

    map<int, int> factors = factorize(N);

    vector<pair<int, int>> result(factors.begin(), factors.end());
    sort(result.rbegin(), result.rend());
    
    for (const auto& factor : result)
        cout << factor.x << " " << factor.y << "\n";
}

int main() {
    optimize();

    int t; 
    cin >> t;
    while(t--)
        solve();
}
