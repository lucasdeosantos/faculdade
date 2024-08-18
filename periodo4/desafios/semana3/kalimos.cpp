#include <bits/stdc++.h>
using namespace std;
#define optimize() ios_base::sync_with_stdio(0); cin.tie();
typedef long long ll;
typedef vector<ll> vll;

ll countMultiples(ll A, ll B, ll x) {
    return (B / x) - ((A - 1) / x);
}

ll inclusionExclusion(ll A, ll B, const vll& primes, int index, ll product, int count) {
    if (index == primes.size())
        return 0;

    ll result = 0;

    result += inclusionExclusion(A, B, primes, index + 1, product, count);

    product *= primes[index];
    if (product <= B) {
        if (count % 2 == 0)
            result += countMultiples(A, B, product);
        else
            result -= countMultiples(A, B, product);

        result += inclusionExclusion(A, B, primes, index + 1, product, count + 1);
    }

    return result;
}

void solve() {
    ll A, B;
    int N;
    cin >> A >> B >> N;

    vll primes(N);
    for (int i = 0; i < N; ++i)
        cin >> primes[i];

    ll totalNumbers = B - A + 1;
    ll kalimos = totalNumbers - inclusionExclusion(A, B, primes, 0, 1, 0);

    cout << kalimos << "\n";
}

int main() {
    optimize();

    int t = 1;
    while(t--)
        solve();
}
