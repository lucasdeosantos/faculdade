#include <bits/stdc++.h>
using namespace std;
#define optimize() ios_base::sync_with_stdio(0); cin.tie();
typedef long long ll;

void solve() {
    int X, Y, A, B, C;
    cin >> X >> Y >> A >> B >> C;

    vector<ll> red(A);
    vector<ll> green(B);
    vector<ll> noColor(C);

    for (int i = 0; i < A; ++i)
        cin >> red[i];

    for (int i = 0; i < B; ++i)
        cin >> green[i];

    for (int i = 0; i < C; ++i)
        cin >> noColor[i];

    sort(red.rbegin(), red.rend());
    sort(green.rbegin(), green.rend());
    sort(noColor.rbegin(), noColor.rend());

    vector<ll> apples;
    
    for (int i = 0; i < X; ++i)
        apples.push_back(red[i]);

    for (int i = 0; i < Y; ++i)
        apples.push_back(green[i]);

    sort(apples.begin(), apples.end());

    ll taste = 0;
    int idx = 0;

    for (int i = 0; i < X + Y; ++i) {
        if (idx < C && noColor[idx] > apples[i])
            taste += noColor[idx++];
        else
            taste += apples[i];
    }

    cout << taste << "\n";
}

int main() {
    optimize();

    int t = 1;
    while(t--)
        solve();
}