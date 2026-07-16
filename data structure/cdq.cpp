#include<bits/stdc++.h>
using namespace std;
struct Point
{
    int p[3];
    int w;
};
struct Query
{
    int p[3];
    int id;
};
vector<Point> points;
vector<Query> queries;
vector<int> ans;
int V;
struct Fenwick
{
    vector<int> c;
    int n;
    Fenwick(){}
    Fenwick(int n) : c(n + 1, 0), n(n) {}
    void upd(int x, int v) {
        for(; x <= n; x += x & -x) c[x] += v;
    }
    int query(int x) {
        int res=0;
        for(; x; x-=x&-x) res+=c[x];
        return res;
    }
}ds;

// 假设目前待处理的点为 points[il,ir]，待处理的询问为 queries[ql,qr]
// d=2 时，需要保证函数结束后按照第 1 维排序
template<int d>void solve(int l, int r, int il, int ir, int ql, int qr);

// d=1 时需保证已排好序
template<>void solve<1>(int l, int r, int il, int ir, int ql, int qr) {
    if(il>ir||ql>qr) return;
    int s=0;
    for(int qi=ql, i=il; qi<=qr; qi++) {
        while(i<=ir && queries[qi].p[0] >= points[i].p[0]) {
            s += points[i].w;
            i++;
        }
        ans[queries[qi].id] += s;
    }
}

vector<Point> tmp_points;
vector<Query> tmp_queries;

template<typename T, typename Comp>
void merge(vector<T> &a, int l, int m, int r, vector<T> &tmp, Comp cmp) {
    int i = l, j = m, k = 0;
    while (i < m && j <= r) {
        if (cmp(a[i], a[j])) tmp[k++] = a[i++];
        else tmp[k++] = a[j++];
    }
    while (i < m) tmp[k++] = a[i++];
    while (j <= r) tmp[k++] = a[j++];
    for (int t = 0; t < k; ++t) a[l + t] = tmp[t];
}
template<typename T, typename Divider>
int divide(vector<T> &a, int l, int r, Divider left) {
    int i = l, j = r;
    while (i < j) {
        while (i < j && left(a[i])) i++;
        while (i < j && !left(a[j])) j--;
        if (i < j) swap(a[i++], a[j--]);
    }
    for(;i<=r&&left(a[i]);i++);
    return i;
}

// 另一种做法：归并，比较慢。
// template<>
// void solve<2>(int l, int r, int il, int ir, int ql, int qr) {
//     if(il>ir||ql>qr||l==r) 
//     {
//         sort(points.begin() + il, points.begin() + ir + 1, [](const Point &a, const Point &b) {
//                 return a.p[0] < b.p[0];
//             });
//         sort(queries.begin() + ql, queries.begin() + qr + 1, [](const Query &a, const Query &b) {
//                 return a.p[0] < b.p[0];
//             });
//         if(l==r) solve<1>(1, V, il, ir, ql, qr);
//         return;
//     }
//     int m = (l + r) / 2;
//     int im=divide(points, il, ir, [m](const Point &x) {
//         return x.p[1] <= m;
//     });
//     int qm=divide(queries, ql, qr, [m](const Query &x) {
//         return x.p[1] <= m;
//     });
//     solve<2>(l, m, il, im - 1, ql, qm - 1);
//     solve<2>(m + 1, r, im, ir, qm, qr);
//     solve<1>(1, V, il, im - 1, qm, qr);
//     merge(points, il, im, ir, tmp_points, [](const Point &a, const Point &b) {
//         return a.p[0] < b.p[0];
//     });
//     merge(queries, ql, qm, qr, tmp_queries, [](const Query &a, const Query &b) {
//         return a.p[0] < b.p[0];
//     });
// }

//使用扫描线完成 d=2 的情况
template<>
void solve<2>(int l, int r, int il, int ir, int ql, int qr) {
    if(il>ir||ql>qr) return;
    //排序可以变成归并，只需让 solve<3> 保证函数结束后按y排序
    sort(points.begin() + il, points.begin() + ir + 1, [](const Point &x, const Point &y) {
        return x.p[1] < y.p[1];
    });
    sort(queries.begin() + ql, queries.begin() + qr + 1, [](const Query &x, const Query &y) {
        return x.p[1] < y.p[1];
    });
    int i=il;
    for(int qi=ql; qi<=qr; qi++) {
        while(i<=ir&&queries[qi].p[1]>=points[i].p[1]) {
            ds.upd(points[i].p[0], points[i].w);
            i++;
        }
        ans[queries[qi].id] += ds.query(queries[qi].p[0]);
    }
    // 清空方法一：逆运算
    for(int j=il; j<i; j++) {
        ds.upd(points[j].p[0], -points[j].w);
    }
    // 清空方法二：增加时间戳，在树状数组修改时记录当前时间戳。若某个位置的记录时间戳与当前时间戳不同则该位置视为空值。适合 max 之类的情况
    // 方法三：坐标离散化后现用现开树状数组
}



template<int d>
void solve(int l, int r, int il, int ir, int ql, int qr) {
    if(il>ir||ql>qr) 
        return;
    if (l == r) {
        solve<d-1>(1, V, il, ir, ql, qr);
        return;
    }
    int m = (l + r) / 2;
    int im=divide(points, il, ir, [&](const Point &x) {
        return x.p[d-1] <= m;
    });
    int qm=divide(queries, ql, qr, [&](const Query &x) {
        return x.p[d-1] <= m;
    });
    solve<d>(l, m, il, im - 1, ql, qm - 1);
    solve<d>(m + 1, r, im, ir, qm, qr);
    solve<d-1>(1, V, il, im - 1, qm, qr);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    // freopen("data.txt", "r", stdin);

    int n;
    cin >> n >> V;
    points.resize(n+1);
    tmp_points.resize(n+1);
    queries.resize(n+1);
    tmp_queries.resize(n+1);
    ans.resize(n+1, 0);
    for(int i=1; i<=n; i++) {
        cin >> points[i].p[0] >> points[i].p[1] >> points[i].p[2];
        points[i].w=1;

        queries[i].p[0] = points[i].p[0];
        queries[i].p[1] = points[i].p[1];
        queries[i].p[2] = points[i].p[2];
        queries[i].id = i;
    }
    ds = Fenwick(V);
    solve<3>(1, V, 1, n, 1, n);
    vector<int>cnt(n);
    for(int i=1; i<=n; i++) {
        cnt[ans[i]-1]++;
    }
    for(int i=0; i<n; i++) {
        cout << cnt[i] << "\n";
    }
}