#include <bits/stdc++.h>
using namespace std;

using int64 = long long;

struct SegTree {
    int n;
    vector<int64> sum, lazy;

    explicit SegTree(const vector<int64>& a) {
        n = (int)a.size() - 1;
        sum.assign(4 * n + 5, 0);
        lazy.assign(4 * n + 5, 0);
        build(1, 1, n, a);
    }

    void build(int p, int l, int r, const vector<int64>& a) {
        if (l == r) {
            sum[p] = a[l];
            return;
        }
        int mid = (l + r) / 2;
        build(p * 2, l, mid, a);
        build(p * 2 + 1, mid + 1, r, a);
        pull(p);
    }

    void pull(int p) {
        sum[p] = sum[p * 2] + sum[p * 2 + 1];
    }

    void apply(int p, int l, int r, int64 v) {
        (void)l;
        (void)r;
        // 故意写错：区间整体加 v 时，sum 应该增加 v * 区间长度。
        // 这里少乘了区间长度，课堂上可以用它演示对拍如何抓 bug。
        sum[p] += v;
        lazy[p] += v;
    }

    void push(int p, int l, int r) {
        if (lazy[p] == 0 || l == r) return;
        int mid = (l + r) / 2;
        apply(p * 2, l, mid, lazy[p]);
        apply(p * 2 + 1, mid + 1, r, lazy[p]);
        lazy[p] = 0;
    }

    void add(int ql, int qr, int64 v) {
        add(1, 1, n, ql, qr, v);
    }

    void add(int p, int l, int r, int ql, int qr, int64 v) {
        if (ql <= l && r <= qr) {
            apply(p, l, r, v);
            return;
        }
        push(p, l, r);
        int mid = (l + r) / 2;
        if (ql <= mid) add(p * 2, l, mid, ql, qr, v);
        if (qr > mid) add(p * 2 + 1, mid + 1, r, ql, qr, v);
        pull(p);
    }

    int64 query(int ql, int qr) {
        return query(1, 1, n, ql, qr);
    }

    int64 query(int p, int l, int r, int ql, int qr) {
        if (ql <= l && r <= qr) return sum[p];
        push(p, l, r);
        int mid = (l + r) / 2;
        int64 ans = 0;
        if (ql <= mid) ans += query(p * 2, l, mid, ql, qr);
        if (qr > mid) ans += query(p * 2 + 1, mid + 1, r, ql, qr);
        return ans;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    if (!(cin >> n >> m)) return 0;

    vector<int64> a(n + 1);
    for (int i = 1; i <= n; i++) cin >> a[i];

    SegTree seg(a);

    for (int i = 0; i < m; i++) {
        int op, l, r;
        cin >> op >> l >> r;
        if (op == 1) {
            int64 x;
            cin >> x;
            seg.add(l, r, x);
        } else if (op == 2) {
            cout << seg.query(l, r) << '\n';
        }
    }
    return 0;
}
