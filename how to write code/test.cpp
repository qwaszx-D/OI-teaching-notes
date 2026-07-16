#include <bits/stdc++.h>
using namespace std;

namespace STD
{
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

void solve(istream& in, ostream& out) {
    int n, m;
    if (!(in >> n >> m)) return;

    vector<int64> a(n + 1);
    for (int i = 1; i <= n; i++) in >> a[i];

    SegTree seg(a);

    for (int i = 0; i < m; i++) {
        int op, l, r;
        in >> op >> l >> r;

        if (op == 1) {
            int64 x;
            in >> x;
            seg.add(l, r, x);
        } else if (op == 2) {
            out << seg.query(l, r) << '\n';
        }
    }
}
}

namespace BRUTE
{
    using int64 = long long;

void solve(istream& in, ostream& out) {
    int n, m;
    if (!(in >> n >> m)) return;

    vector<int64> a(n + 1);
    for (int i = 1; i <= n; i++) in >> a[i];

    for (int i = 0; i < m; i++) {
        int op, l, r;
        in >> op >> l >> r;
        if (op == 1) {
            int64 x;
            in >> x;
            for (int j = l; j <= r; j++) a[j] += x;
        } else if (op == 2) {
            int64 ans = 0;
            for (int j = l; j <= r; j++) ans += a[j];
            out << ans << '\n';
        }
    }
}
}

namespace GEN
{
    string genInput(uint64_t seed) {
    mt19937_64 rng(seed);

    auto rnd = [&](int l, int r) -> int {
        return uniform_int_distribution<int>(l, r)(rng);
    };

    ostringstream out;

    int N = 100;

    int n;
    if (rnd(1, 10) == 1) n = 1;
    else n = rnd(2, N);

    int m = rnd(1, N);

    out << n << ' ' << m << '\n';

    for (int i = 1; i <= n; i++) {
        if (i > 1) out << ' ';
        out << rnd(-N, N);
    }
    out << '\n';

    for (int i = 1; i <= m; i++) {
        int l = rnd(1, n), r = rnd(1, n);
        if (l > r) swap(l, r);

        int op = (rnd(1, 10) <= 6 ? 1 : 2);

        if (op == 1) {
            int x = rnd(-N, N);
            out << 1 << ' ' << l << ' ' << r << ' ' << x << '\n';
        } else {
            out << 2 << ' ' << l << ' ' << r << '\n';
        }
    }

    return out.str();
}
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    uint64_t base_seed = chrono::steady_clock::now().time_since_epoch().count();

    for (int tc = 1; ; tc++) {
        uint64_t seed = base_seed + tc * 998244353ull;

        string input = GEN::genInput(seed);

        istringstream in_std(input), in_brute(input);
        ostringstream out_std, out_brute;

        STD::solve(in_std, out_std);
        BRUTE::solve(in_brute, out_brute);

        if (out_std.str() != out_brute.str()) {
            cerr << "Wrong Answer!\n";
            cerr << "testcase = " << tc << '\n';
            cerr << "seed = " << seed << '\n';

            cerr << "\nInput:\n";
            cerr << input;

            cerr << "\nStd output:\n";
            cerr << out_std.str();

            cerr << "\nBrute output:\n";
            cerr << out_brute.str();

            return 0;
        }

        if (tc % 1000 == 0) {
            cerr << "Accepted " << tc << " tests\n";
        }
    }

    return 0;
}