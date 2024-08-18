#include <bits/stdc++.h>
using namespace std;
#define optimize() ios_base::sync_with_stdio(0); cin.tie();
typedef long long ll;
typedef vector<int> vi;

double findHeight(const vi& lengths, ll A) {
    double low = 0;
    double high = *max_element(lengths.begin(), lengths.end());
    const double eps = 1e-8;

    while (high - low > eps) {
        double mid = (low + high) / 2.0;
        ll areaCut = 0;
        
        for (int length : lengths)
            if (length > mid)
                areaCut += (length - mid);
        
        if (areaCut >= A)
            low = mid;
        else
            high = mid;
    }
    
    ll areaCut = 0;
    for (int length : lengths)
        if (length > low)
            areaCut += (length - low);
    
    if (areaCut < A)
        return -1.0;
    
    return low;
}

void solve() {
    int N;
    ll A;
    
    while (cin >> N >> A, N != 0 || A != 0) {
        vi lengths(N);

        for (int i = 0; i < N; ++i)
            cin >> lengths[i];
        
        if (A == 0) {
            cout << fixed << setprecision(4) << *max_element(lengths.begin(), lengths.end()) + 0.0001 << "\n";
        } 
        else {
            double result = findHeight(lengths, A);
            if (result < 0.0)
                cout << "-.-" << "\n";
            else if (result == 0.0)
                cout << ":D" << "\n";
            else
                cout << fixed << setprecision(4) << result << "\n";
        }
    }
}

int main() {
    optimize();

    int t = 1;
    while(t--)
        solve();
}
