#include <bits/stdc++.h>
using namespace std;
#define optimize() ios_base::sync_with_stdio(0); cin.tie();
typedef vector<int> vi;

const int MAX_N = 12;

vi adj[MAX_N];
vi in_degree(MAX_N, 0);
vi order;
set<vi> results;

void dfs(int depth, int N) {
    if (depth == N) {
        results.insert(order);
        return;
    }
    
    for (int i = 0; i < N; ++i) {
        if (in_degree[i] == 0 && find(order.begin(), order.end(), i) == order.end()) {
            order.push_back(i);
            for (int neighbor : adj[i])
                --in_degree[neighbor];

            dfs(depth + 1, N);
            order.pop_back();
            for (int neighbor : adj[i])
                ++in_degree[neighbor];
        }
    }
}

void solve() {
    int N, C;
    cin >> N >> C;
    
    for (int i = 0; i < C; ++i) {
        int A, B;
        cin >> A >> B;
        --A;
        --B;
        adj[A].push_back(B);
        ++in_degree[B];
    }
    
    order.reserve(N);
    dfs(0, N);
    
    for (const auto& perm : results) {
        for (int i = 0; i < N; ++i)
            cout << perm[i] + 1 << " ";
        cout << "\n";
    }
}

int main() {
    optimize();

    int t = 1;
    while(t--)
        solve();
}
