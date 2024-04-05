#include <bits/stdc++.h>
using namespace std;
#define optimize() ios_base::sync_with_stdio(0); cin.tie();

void solve() {
    int N;
    cin >> N;
    vector<string> strings(N);

    for (int i = 0; i < N; ++i)
        cin >> strings[i];

    sort(strings.begin(), strings.end(), [](const string &a, const string &b){
        return (a + b) < (b + a);
    });

    string result;

    for (int i = 0; i < N; ++i)
        result += strings[i];

    cout << result;
}

int main() {
    optimize();

    int t = 1;
    while (t--)
        solve();
}
