#include <bits/stdc++.h>
using namespace std;
#define optimize() ios_base::sync_with_stdio(0); cin.tie();

void solve() {
        int P, H, F, D, C;
        cin >> P >> H >> F >> D >> C;

        int maxProfit = 0;

        for (int i = 0; i <= min(P / 2, H); i++) {
            int remainingP = P - i * 2;
            int remainingH = H - i;
            int profit = i * D;

            int numChicken = min(remainingP / 2, F);
            profit += numChicken * C;

            maxProfit = max(maxProfit, profit);
        }

        for (int i = 0; i <= min(P / 2, F); i++) {
            int remainingP = P - i * 2;
            int remainingF = F - i;
            int profit = i * C;

            int numCheddar = min(remainingP / 2, H);
            profit += numCheddar * D;

            maxProfit = max(maxProfit, profit);
        }

        cout << maxProfit << endl;
}

int main() {
    optimize();

    int t;
    cin >> t;
    while (t--)
        solve();
}
