#include <bits/stdc++.h>
using namespace std;
#define optimize() ios_base::sync_with_stdio(0); cin.tie();
typedef long long ll;
typedef vector<ll> vl;
typedef vector<bool> vb;

vl generatePrimes(int N) {
    vb isPrime(N + 1, true);
    vl primes;

    isPrime[0] = isPrime[1] = false;

    for (int i = 2; i <= N; ++i)
        if (isPrime[i]) {
            primes.push_back(i);
            for (int j = 2 * i; j <= N; j += i)
                isPrime[j] = false;
        }

    return primes;
}

void solve() {
    int N;
    cin >> N;

    vl primos = generatePrimes(N);
    int count = 0;

    for (int i = 0; i < primos.size(); ++i) {
        int p1 = primos[i];
        int p2 = N - p1;
        if (p2 >= p1 && std::binary_search(primos.begin(), primos.end(), p2))
            count++;
    }

    cout << count << "\n";
}

int main() {
    optimize();

    int t = 1;
    while(t--)
        solve();
}
