#include <bits/stdc++.h>
using namespace std;
#define optimize() ios_base::sync_with_stdio(0); cin.tie();
typedef vector<int> vi;

int calculateOR(const vi& arr, int start, int end) {
    int result = 0;
    for (int i = start; i <= end; ++i)
        result |= arr[i];

    return result;
}

void findMinXOR(const vi& arr, int index, vi& currentORs, int& minXOR) {
    if (index == arr.size()) {
        int xorValue = 0;
        for (int orValue : currentORs)
            xorValue ^= orValue;

        minXOR = min(minXOR, xorValue);
        return;
    }
    
    int orValue = 0;
    for (int i = index; i < arr.size(); ++i) {
        orValue |= arr[i];
        currentORs.push_back(orValue);
        findMinXOR(arr, i + 1, currentORs, minXOR);
        currentORs.pop_back();
    }
}

void solve() {
    int N;
    cin >> N;
    
    vi arr(N);
    for (int i = 0; i < N; ++i)
        cin >> arr[i];

    int minXOR = numeric_limits<int>::max();
    vi currentORs;
    findMinXOR(arr, 0, currentORs, minXOR);

    cout << minXOR << "\n";
}

int main() {
    optimize();

    int t = 1;
    while(t--)
        solve();
}
