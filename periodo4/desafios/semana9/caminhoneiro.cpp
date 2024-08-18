#include <bits/stdc++.h>
using namespace std;
#define optimize() ios_base::sync_with_stdio(0); cin.tie();
typedef vector<int> vi;

const int INF = 1e9;

void solve() {
    int N, M;
    cin >> N >> M;

    vector<vi> dist(N, vi(N, INF));

    for (int i = 0; i < M; ++i) {
        int u, v, w;
        cin >> u >> v >> w;
        dist[u][v] = min(dist[u][v], w);
        dist[v][u] = min(dist[v][u], w);
    }

    for (int i = 0; i < N; ++i)
        dist[i][i] = 0;

    for (int k = 0; k < N; ++k)
        for (int i = 0; i < N; ++i)
            for (int j = 0; j < N; ++j)
                if (dist[i][k] < INF && dist[k][j] < INF)
                    dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);

    int best_max_dist = INF;
    for (int i = 0; i < N; ++i) {
        int max_dist = 0;
        for (int j = 0; j < N; ++j)
            max_dist = max(max_dist, dist[i][j]);
        best_max_dist = min(best_max_dist, max_dist);
    }

    cout << best_max_dist << "\n";
}

int main() {
    optimize();

    int t = 1;
    while(t--)
        solve();
}
