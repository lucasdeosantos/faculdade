#include <bits/stdc++.h>
using namespace std;
#define optimize() ios_base::sync_with_stdio(0); cin.tie();
typedef vector<int> vi;
typedef vector<bool> vb;

void dfs(int v, const vector<vi>& adj, vb& visited) {
    visited[v] = true;

    for (int u : adj[v])
        if (!visited[u])
            dfs(u, adj, visited);
}

void solve() {
    int N, M;
    cin >> N >> M;
    
    vector<vi> adj(N);
    
    for (int i = 0; i < M; ++i) {
        int A, B;
        cin >> A >> B;
        adj[A].push_back(B);
        adj[B].push_back(A);
    }
    
    vb visited(N, false);
    int numComponents = 0;
    
    for (int i = 0; i < N; ++i)
        if (!visited[i]) {
            dfs(i, adj, visited);
            ++numComponents;
        }
    
    cout << numComponents << "\n";
}

int main() {
    optimize();

    int t = 1;
    while(t--)
        solve();
}
