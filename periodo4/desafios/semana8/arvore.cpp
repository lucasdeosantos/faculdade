#include <bits/stdc++.h>
using namespace std;
#define optimize() ios_base::sync_with_stdio(0); cin.tie();
typedef vector<int> vi;
typedef queue<int> qi;

vi bfs(const vector<vi>& adj, int start, int& furthestNode) {
    vi dist(adj.size(), -1);
    qi q;
    q.push(start);
    dist[start] = 0;
    
    int maxDist = 0;
    
    while (!q.empty()) {
        int node = q.front();
        q.pop();
        
        for (int neighbor : adj[node])
            if (dist[neighbor] == -1) {
                dist[neighbor] = dist[node] + 1;
                q.push(neighbor);
                
                if (dist[neighbor] > maxDist) {
                    maxDist = dist[neighbor];
                    furthestNode = neighbor;
                }
            }
    }
    
    return dist;
}

void solve() {
    int N;
    cin >> N;
    
    vector<vi> adj(N + 1);
    
    for (int i = 0; i < N - 1; ++i) {
        int A, B;
        cin >> A >> B;
        adj[A].push_back(B);
        adj[B].push_back(A);
    }
    
    int furthestNode = 1;
    bfs(adj, 1, furthestNode);
    
    vi dist = bfs(adj, furthestNode, furthestNode);
    
    cout << *max_element(dist.begin(), dist.end()) << "\n";
}

int main() {
    optimize();

    int t = 1;
    while(t--)
        solve();
}
