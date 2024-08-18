#include <bits/stdc++.h>
using namespace std;
#define optimize() ios_base::sync_with_stdio(0); cin.tie();
typedef vector<int> vi;

class SegmentTree {
private:
    vi tree, lazy;
    int n;

    void build(int node, int start, int end, const vi &arr) {
        if (start == end) {
            tree[node] = arr[start];
        } 
        else {
            int mid = (start + end) / 2;
            build(2 * node + 1, start, mid, arr);
            build(2 * node + 2, mid + 1, end, arr);
            tree[node] = min(tree[2 * node + 1], tree[2 * node + 2]);
        }
    }

    void updateRange(int node, int start, int end, int l, int r, int val) {
        if (lazy[node] != 0) {
            tree[node] += lazy[node];
            if (start != end) {
                lazy[2 * node + 1] += lazy[node];
                lazy[2 * node + 2] += lazy[node];
            }
            lazy[node] = 0;
        }

        if (start > r || end < l) return;

        if (start >= l && end <= r) {
            tree[node] += val;
            if (start != end) {
                lazy[2 * node + 1] += val;
                lazy[2 * node + 2] += val;
            }
            return;
        }

        int mid = (start + end) / 2;
        updateRange(2 * node + 1, start, mid, l, r, val);
        updateRange(2 * node + 2, mid + 1, end, l, r, val);
        tree[node] = min(tree[2 * node + 1], tree[2 * node + 2]);
    }

    int queryRange(int node, int start, int end, int l, int r) {
        if (lazy[node] != 0) {
            tree[node] += lazy[node];
            if (start != end) {
                lazy[2 * node + 1] += lazy[node];
                lazy[2 * node + 2] += lazy[node];
            }
            lazy[node] = 0;
        }

        if (start > r || end < l) return INT_MAX;

        if (start >= l && end <= r) return tree[node];

        int mid = (start + end) / 2;
        int left_query = queryRange(2 * node + 1, start, mid, l, r);
        int right_query = queryRange(2 * node + 2, mid + 1, end, l, r);

        return min(left_query, right_query);
    }

public:
    SegmentTree(const vector<int> &arr) {
        n = arr.size();
        tree.resize(4 * n);
        lazy.resize(4 * n, 0);
        build(0, 0, n - 1, arr);
    }

    void updateRange(int l, int r, int val) {
        updateRange(0, 0, n - 1, l, r, val);
    }

    int queryRange(int l, int r) {
        return queryRange(0, 0, n - 1, l, r);
    }
};

int sign(int x) {
    if (x > 0) return 1;
    if (x < 0) return -1;
    return 0;
}

void solve() {
    int N, Q;
    cin >> N >> Q;

    vector<int> arr(N);
    for (int i = 0; i < N; ++i)
        cin >> arr[i];

    SegmentTree segTree(arr);

    while (Q--) {
        int type, I, J, K;
        cin >> type >> I >> J;
        I--; 
        J--;

        if (type == 1) {
            cin >> K;
            segTree.updateRange(I, J, K);
        } 
        else if (type == 2) {
            cout << segTree.queryRange(I, J) << endl;
        }
    }
}

int main() {
    optimize();

    int t = 1;
    while(t--)
        solve();
}
