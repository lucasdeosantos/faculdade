#include <bits/stdc++.h>
using namespace std;
#define optimize() ios_base::sync_with_stdio(0); cin.tie();
typedef vector<int> vi;

vector<vi> adj;
vi subtreeSize;

void dfs1(int node, int parent) {
    subtreeSize[node] = 1;

    for (int neighbor : adj[node])
        if (neighbor != parent) {
            dfs1(neighbor, node);
            subtreeSize[node] += subtreeSize[neighbor];
        }
}

int findCentroid(int node, int parent, int totalSize) {
    for (int neighbor : adj[node])
        if (neighbor != parent && subtreeSize[neighbor] > totalSize / 2)
            return findCentroid(neighbor, node, totalSize);

    return node;
}

void solve() {
    int N;
    cin >> N;

    adj.resize(N + 1);
    subtreeSize.resize(N + 1);

    for (int i = 0; i < N - 1; ++i) {
        int A, B;
        cin >> A >> B;
        adj[A].push_back(B);
        adj[B].push_back(A);
    }

    dfs1(1, -1);

    int centroid = findCentroid(1, -1, subtreeSize[1]);

    cout << centroid << "\n";
}

int main() {
    optimize();

    int t = 1;
    while(t--)
        solve();
}
