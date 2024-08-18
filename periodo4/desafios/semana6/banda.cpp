#include <bits/stdc++.h>
using namespace std;
#define optimize() ios_base::sync_with_stdio(0); cin.tie();
typedef long long ll;

void solve() {
    int N, M;
    cin >> N >> M;

    multiset<ll> tickets;
    for (int i = 0; i < N; ++i) {
        ll h;
        cin >> h;
        tickets.insert(h);
    }

    for (int i = 0; i < M; ++i) {
        ll t;
        cin >> t;
        auto it = tickets.upper_bound(t);

        if (it == tickets.begin()) {
            cout << -1 << "\n";
        } 
        else {
            --it;
            cout << *it << "\n";
            tickets.erase(it);
        }
    }
}

int main() {
    optimize();

    int t = 1;
    while(t--)
        solve();
}
