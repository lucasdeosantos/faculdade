#include <bits/stdc++.h>
using namespace std;
#define optimize() ios_base::sync_with_stdio(0); cin.tie();
typedef vector<int> vi;

vi computePrefixFunction(const string &s) {
    int n = s.length();
    vi pi(n);
    for (int i = 1; i < n; ++i) {
        int j = pi[i - 1];
        while (j > 0 && s[i] != s[j]) j = pi[j - 1];
        if (s[i] == s[j]) ++j;
        pi[i] = j;
    }
    return pi;
}

void solve() {
    string s;
    cin >> s;

    vi pi = computePrefixFunction(s);
    int n = s.length();
    
    vi borders;
    int k = pi[n - 1];
    while (k > 0) {
        borders.push_back(k);
        k = pi[k - 1];
    }

    sort(borders.begin(), borders.end());

    for (int len : borders)
        cout << len << " ";

    cout << "\n";
}

int main() {
    optimize();

    int t = 1;
    while(t--)
        solve();
}
