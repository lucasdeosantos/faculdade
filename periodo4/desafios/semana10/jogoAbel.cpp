#include <bits/stdc++.h>
using namespace std;
#define optimize() ios_base::sync_with_stdio(0); cin.tie();
typedef vector<int> vi;

class SegmentTree {
private:
    vi tree;
    int n;

    int build(int node, int start, int end, const vi &arr) {
        if (start == end)
            return tree[node] = arr[start];

        int mid = (start + end) / 2;
        int left_product = build(2 * node + 1, start, mid, arr);
        int right_product = build(2 * node + 2, mid + 1, end, arr);

        return tree[node] = left_product * right_product;
    }

    void update(int node, int start, int end, int idx, int value) {
        if (start == end) {
            tree[node] = value;
        } 
        else {
            int mid = (start + end) / 2;
            if (idx <= mid)
                update(2 * node + 1, start, mid, idx, value);
            else
                update(2 * node + 2, mid + 1, end, idx, value);

            tree[node] = tree[2 * node + 1] * tree[2 * node + 2];
        }
    }

    int query(int node, int start, int end, int l, int r) {
        if (r < start || end < l)
            return 1;

        if (l <= start && end <= r)
            return tree[node];

        int mid = (start + end) / 2;
        int left_query = query(2 * node + 1, start, mid, l, r);
        int right_query = query(2 * node + 2, mid + 1, end, l, r);

        return left_query * right_query;
    }

public:
    SegmentTree(const vector<int> &arr) {
        n = arr.size();
        tree.resize(4 * n);
        build(0, 0, n - 1, arr);
    }

    void update(int idx, int value) {
        update(0, 0, n - 1, idx, value);
    }

    int query(int l, int r) {
        return query(0, 0, n - 1, l, r);
    }
};

int sign(int x) {
    if (x > 0) return 1;
    if (x < 0) return -1;
    return 0;
}

void solve() {
    int n, k;
    cin >> n >> k;
    vi arr(n);
    for (int i = 0; i < n; ++i) {
        int x;
        cin >> x;
        arr[i] = sign(x);
    }

    SegmentTree segTree(arr);

    string result;
    for (int i = 0; i < k; ++i) {
        char type;
        int a, b;
        cin >> type >> a >> b;
        if (type == 'A') {
            segTree.update(a - 1, sign(b));
        } 
        else if (type == 'B') {
            int res = segTree.query(a - 1, b - 1);
            if (res > 0) result += '+';
            else if (res < 0) result += '-';
            else result += '0';
        }
    }

    cout << result << "\n";
}

int main() {
    optimize();

    int t = 1;
    while(t--)
        solve();
}
