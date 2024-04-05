#include <bits/stdc++.h>
using namespace std;
#define optimize() ios_base::sync_with_stdio(0); cin.tie();
typedef long long ll;

void solve() {
    int N;
    cin >> N;
    vector<ll> sticks(N);

    for (int i = 0; i < N; ++i)
        cin >> sticks[i];

    sort(sticks.begin(), sticks.end());

    ll m = sticks[N/2];

    ll cost = 0;
    for (int i = 0; i < N; ++i)
        cost += abs(sticks[i] - m);

    cout << cost;
}

int main() {
    optimize();

    int t = 1;
    while (t--)
        solve();
}