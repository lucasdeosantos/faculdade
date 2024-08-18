#include <bits/stdc++.h>
using namespace std;
#define optimize() ios_base::sync_with_stdio(0); cin.tie();
typedef vector<int> vi;

vi fractionToContinuedFraction(int N, int D) {
    vi result;
    
    while (D != 0) {
        int quotient = N / D;
        result.push_back(quotient);
        int remainder = N % D;
        N = D;
        D = remainder;
    }
    
    if (!result.empty() && result.back() == 1 && result.size() > 1) {
        result[result.size() - 2] += 1;
        result.pop_back();
    }
    
    return result;
}

void solve() {
        int N, D;
        cin >> N >> D;
        
        vi result = fractionToContinuedFraction(N, D);
        
        cout << "[" << result[0];
        for (size_t i = 1; i < result.size(); ++i) {
            if (i == 1)
                cout << ";" << result[i];
            else
                cout << "," << result[i];
        }
        cout << "]" << "\n";
}

int main() {
    optimize();

    int t; 
    cin >> t;
    while(t--)
        solve();
}
