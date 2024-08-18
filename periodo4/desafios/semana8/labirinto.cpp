#include <bits/stdc++.h>
using namespace std;
#define optimize() ios_base::sync_with_stdio(0); cin.tie();
#define x first
#define y second
typedef vector<int> vi;
typedef vector<string> vs;
typedef pair<int, int> pi;
typedef vector<pi> vpi;

void solve() {
    int N, M;
    cin >> N >> M;

    vs labirinto(N);
    pi start, end;
    
    for (int i = 0; i < N; ++i) {
        cin >> labirinto[i];
        for (int j = 0; j < M; ++j) {
            if (labirinto[i][j] == 'A')
                start = {i, j};
            else if (labirinto[i][j] == 'B')
                end = {i, j};
        }
    }

    vector<vi> dist(N, vector<int>(M, -1));
    vector<vpi> prev(N, vector<pi>(M, {-1, -1}));
    queue<pi> q;

    const vpi directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
    const string dirChars = "UDLR";

    dist[start.x][start.y] = 0;
    q.push(start);

    while (!q.empty()) {
        auto current = q.front();
        q.pop();

        for (int i = 0; i < 4; ++i) {
            auto next = make_pair(current.x + directions[i].x, current.y + directions[i].y);
            if (next.x >= 0 && next.x < N && next.y >= 0 && next.y < M && labirinto[next.x][next.y] != '#' && dist[next.x][next.y] == -1) {
                dist[next.x][next.y] = dist[current.x][current.y] + 1;
                prev[next.x][next.y] = current;
                q.push(next);
                if (next == end) break;
            }
        }
    }

    if (dist[end.x][end.y] != -1) {
        cout << "YES\n";
        cout << dist[end.x][end.y] << "\n";
        
        string path;
        auto current = end;

        while (current != start) {
            auto p = prev[current.x][current.y];
            if (p.x == current.x - 1 && p.y == current.y) path += 'D';
            else if (p.x == current.x + 1 && p.y == current.y) path += 'U';
            else if (p.x == current.x && p.y == current.y - 1) path += 'R';
            else if (p.x == current.x && p.y == current.y + 1) path += 'L';
            current = p;
        }
        
        reverse(path.begin(), path.end());
        cout << path << "\n";
    } 
    else {
        cout << "NO\n";
    }
}

int main() {
    optimize();

    int t = 1;
    while(t--)
        solve();
}
