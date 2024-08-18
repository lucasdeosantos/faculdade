#include <bits/stdc++.h>
using namespace std;
#define optimize() ios_base::sync_with_stdio(0); cin.tie();
typedef long long ll;
typedef vector<ll> vll;
typedef vector<int> vi;
typedef vector<bool> vb;

struct SegmentTree {
    vll st, lazy_add, lazy_set;
    vb is_lazy_set;
    int n;

    SegmentTree(int n) : n(n) {
        st.resize(4 * n);
        lazy_add.resize(4 * n, 0);
        lazy_set.resize(4 * n, 0);
        is_lazy_set.resize(4 * n, false);
    }

    void propagate(int node, int start, int end) {
        if (is_lazy_set[node]) {
            st[node] = (end - start + 1) * lazy_set[node];
            if (start != end) {
                lazy_set[node * 2] = lazy_set[node * 2 + 1] = lazy_set[node];
                lazy_add[node * 2] = lazy_add[node * 2 + 1] = 0;
                is_lazy_set[node * 2] = is_lazy_set[node * 2 + 1] = true;
            }
            lazy_set[node] = 0;
            is_lazy_set[node] = false;
        }
        if (lazy_add[node] != 0) {
            st[node] += (end - start + 1) * lazy_add[node];
            if (start != end) {
                lazy_add[node * 2] += lazy_add[node];
                lazy_add[node * 2 + 1] += lazy_add[node];
            }
            lazy_add[node] = 0;
        }
    }

    void update_range_add(int node, int start, int end, int l, int r, ll val) {
        propagate(node, start, end);
        if (start > r || end < l) return;
        if (start >= l && end <= r) {
            lazy_add[node] += val;
            propagate(node, start, end);
            return;
        }
        int mid = (start + end) / 2;
        update_range_add(node * 2, start, mid, l, r, val);
        update_range_add(node * 2 + 1, mid + 1, end, l, r, val);
        st[node] = st[node * 2] + st[node * 2 + 1];
    }

    void update_range_set(int node, int start, int end, int l, int r, ll val) {
        propagate(node, start, end);
        if (start > r || end < l) return;
        if (start >= l && end <= r) {
            lazy_set[node] = val;
            is_lazy_set[node] = true;
            propagate(node, start, end);
            return;
        }
        int mid = (start + end) / 2;
        update_range_set(node * 2, start, mid, l, r, val);
        update_range_set(node * 2 + 1, mid + 1, end, l, r, val);
        st[node] = st[node * 2] + st[node * 2 + 1];
    }

    ll query_range(int node, int start, int end, int l, int r) {
        propagate(node, start, end);
        if (start > r || end < l) return 0;
        if (start >= l && end <= r) return st[node];
        int mid = (start + end) / 2;
        return query_range(node * 2, start, mid, l, r) + query_range(node * 2 + 1, mid + 1, end, l, r);
    }

    void update_add(int l, int r, ll val) {
        update_range_add(1, 0, n - 1, l, r, val);
    }

    void update_set(int l, int r, ll val) {
        update_range_set(1, 0, n - 1, l, r, val);
    }

    ll query(int l, int r) {
        return query_range(1, 0, n - 1, l, r);
    }
};

void solve() {
    int n, q;
    cin >> n >> q;
    vi arr(n);

    for (int i = 0; i < n; ++i)
        cin >> arr[i];

    SegmentTree segTree(n);
    segTree.update_set(0, n - 1, arr[0]);
    for (int i = 1; i < n; ++i)
        segTree.update_add(i, i, arr[i] - arr[0]);

    for (int i = 0; i < q; ++i) {
        int type, a, b, x;
        cin >> type >> a >> b;
        --a;
        --b;
        if (type == 1) {
            cin >> x;
            segTree.update_add(a, b, x);
        } 
        else if (type == 2) {
            cin >> x;
            segTree.update_set(a, b, x);
        }
        else if (type == 3) {
            cout << segTree.query(a, b) << endl;
        }
    }
}

int main() {
    optimize();

    int t = 1;
    while(t--)
        solve();
}
