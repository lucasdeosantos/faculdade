#include <bits/stdc++.h>
using namespace std;
#define optimize() ios_base::sync_with_stdio(0); cin.tie();

struct Vaca {
    int L;
    int D;
};

bool compare(const Vaca &a, const Vaca &b) {
    return a.D < b.D;
}

void solve() {
    int N;
    cin >> N;

    vector<Vaca> vacas(N);
    for (int i = 0; i < N; ++i)
        cin >> vacas[i].L >> vacas[i].D;

    sort(vacas.begin(), vacas.end(), compare);

    priority_queue<int, vector<int>, greater<int>> pq;
    int total_leite = 0;

    for (int i = 0; i < N; ++i) {
        if (pq.size() < vacas[i].D) {
            pq.push(vacas[i].L);
            total_leite += vacas[i].L;
        } 
        else if (!pq.empty() && pq.top() < vacas[i].L) {
            total_leite += vacas[i].L - pq.top();
            pq.pop();
            pq.push(vacas[i].L);
        }
    }

    cout << total_leite << "\n";
}

int main() {
    optimize();

    int t = 1;
    while (t--)
        solve();
}
