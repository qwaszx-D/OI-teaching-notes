#include <bits/stdc++.h>
using namespace std;

using int64 = long long;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    if (!(cin >> n >> m)) return 0;

    vector<int64> a(n + 1);
    for (int i = 1; i <= n; i++) cin >> a[i];

    for (int i = 0; i < m; i++) {
        int op, l, r;
        cin >> op >> l >> r;
        if (op == 1) {
            int64 x;
            cin >> x;
            for (int j = l; j <= r; j++) a[j] += x;
        } else if (op == 2) {
            int64 ans = 0;
            for (int j = l; j <= r; j++) ans += a[j];
            cout << ans << '\n';
        }
    }
    return 0;
}
