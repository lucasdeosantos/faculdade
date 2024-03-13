#include <bits/stdc++.h>
using namespace std;

#define altura first
#define sentido second

int main() {
    cin.tie();
    ios_base::sync_with_stdio(0);

    int n;
    cin >> n;
    stack <pair<int, char>> torres;

    for (int i = 0; i < n; i++) {
        int a;
        char d;
        cin >> a >> d;

        if (torres.empty() || (torres.top().altura > a && torres.top().sentido == 'L' && d == 'L'))
            torres.push({a, d});
        else if (torres.top().altura == a && torres.top().sentido == 'L' && d == 'O')
            torres.pop();
        else
            break;
    }

    if (torres.empty())
        cout << "S" << "\n";
    else
        cout << "N" << "\n";
}
