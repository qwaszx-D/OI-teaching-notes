#include <bits/stdc++.h>
using namespace std;

int main() {
    uint64_t seed;

    //找一个合适的毫秒级的时钟
    seed = chrono::steady_clock::now().time_since_epoch().count();
    
    mt19937_64 rng(seed);

    auto rnd = [&](int l, int r) -> int {
        return uniform_int_distribution<int>(l, r)(rng);
    };

    // 首先读入 n,m
    // 然后读入 n 个数表示初始序列 a
    // 然后进行 m 次操作
    // 每次操作有两种类型：
    // 1. 1 l r x：将区间 [l,r] 上的数加上 x
    // 2. 2 l r：询问区间 [l,r] 上的数的和


    // 对拍时数据要小一些，因为 brute.cpp 是 O(nm) 的。
    int N=100;
    int n;
    if (rnd(1, 10) == 1) n = 1;          // 特意多测 n=1 的边界情况
    else n = rnd(2, N);

    int m = rnd(1, N);

    cout << n << ' ' << m << '\n';
    for (int i = 1; i <= n; i++) {
        if (i > 1) cout << ' ';
        cout << rnd(-N, N);
    }
    cout << '\n';

    // 先随机 l，然后在 [l,n] 里随机 r  (r-l+1) 的期望是 n/4
    // 随机 l,r。如果 l>r 则交换。  (r-l+1) 的期望是 n/3
    // 这两种随机的分布不一样

    // 树
    // 对每个 i，随机一个 [1,i-1] 作为父亲。最后打乱标号。
    // 树高期望 (同时也是高概率) O(logn)
    // 每次随机 u,v，如果 u,v 不连通则连边
    // 树高期望 O(sqrtn)

    // 有人证明了 采样 和 计数 差不多是等难的

    for (int i = 1; i <= m; i++) {
        int l = rnd(1, n), r = rnd(1, n);
        if (l > r) swap(l, r);

        // 大约 60% 是修改，40% 是查询。
        int op = (rnd(1, 10) <= 6 ? 1 : 2);
        if (op == 1) {
            int x = rnd(-N, N);
            cout << 1 << ' ' << l << ' ' << r << ' ' << x << '\n';
        } else {
            cout << 2 << ' ' << l << ' ' << r << '\n';
        }
    }
    return 0;
}
