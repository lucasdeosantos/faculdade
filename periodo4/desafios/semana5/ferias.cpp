#include <bits/stdc++.h>
using namespace std;
#define optimize() ios_base::sync_with_stdio(0); cin.tie();
typedef long long ll;

void solve() {
    int N;
    cin >> N;

    vector<ll> A(N), B(N), C(N);

    for(int i = 0; i < N; ++i)
        cin >> A[i] >> B[i] >> C[i];

    vector<ll> dpA(N), dpB(N), dpC(N);

    dpA[0] = A[0];
    dpB[0] = B[0];
    dpC[0] = C[0];
    
    for (int i = 1; i < N; ++i) {
        dpA[i] = A[i] + max(dpB[i-1], dpC[i-1]);
        dpB[i] = B[i] + max(dpA[i-1], dpC[i-1]);
        dpC[i] = C[i] + max(dpA[i-1], dpB[i-1]);
    }

    int maxHappiness = max({dpA[N-1], dpB[N-1], dpC[N-1]});

    cout << maxHappiness << "\n";
}

int main() {
    optimize();

    int t = 1;
    while(t--)
        solve();
}
