#include <bits/stdc++.h>
using namespace std;
#define optimize() ios_base::sync_with_stdio(0); cin.tie();
typedef unsigned long long ull;

unordered_map<ull, ull> mem;

ull f(ull N) {
    if (!N) 
        return 1;
    if(mem.find(N) != mem.end()) 
        return mem[N];

    mem[N] = f(N/2) + f(N/3);
    return mem[N];
}

void solve() {
    ull N;
    cin >> N;
    cout << f(N) << "\n";
}

int main() {
    optimize();

    int t = 1;
    while(t--)
        solve();
}
