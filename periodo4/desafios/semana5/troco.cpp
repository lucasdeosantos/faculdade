#include <bits/stdc++.h>
using namespace std;
#define optimize() ios_base::sync_with_stdio(0); cin.tie();
typedef long long ll;

vector<int> coins = {1, 5, 10, 25, 50};
const int MAX_N = 30000;

void solve() {

    vector<ll> dp(MAX_N + 1, 0);

    dp[0] = 1;

    for (ll coin : coins) {
        for (ll i = coin; i <= MAX_N; ++i) {
            dp[i] += dp[i - coin];
        }
    }

    int N;
    while (cin >> N)
        cout << dp[N] << "\n";
}

int main() {
    optimize();

    int t = 1;
    while(t--)
        solve();
}
