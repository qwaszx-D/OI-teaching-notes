// 洛谷 P3834：区间第 k 小
// 整体二分

#include <bits/stdc++.h>
using namespace std;

struct Query {
	int l, r, k, id;
};
vector<Query> queries;

struct Item {
    int pos, val;
};
vector<Item> a;

struct QueryItem
{
    int pos, coef, id;
};
vector<QueryItem> queryItems;
vector<int> ans;
vector<int> s;

// // 1log
// //注意这里的划分函数必须保持 pos 的顺序，所以只能用这种方式。
// template<typename T, typename Divider>
// int divide(vector<T> &a, int l, int r, Divider left) {
//     static vector<T> tmp;
//     tmp.clear();
//     tmp.reserve(r - l + 1);
//     int il=l;
//     for(int i = l; i <= r; i++)
//         if(left(a[i]))
//             a[il++]=a[i];
//         else tmp.push_back(a[i]);
//     for(int i=0; i<(int)tmp.size(); i++)
//         a[il+i]=tmp[i];
//     return il;
// }
// // 离散化 pos
// int reindex(int il, int ir, int ql, int qr)
// {
//     int tn=0, i=il;
//     for(int qi=ql;qi<=qr;qi++)
//     {
//         if(i<=ir && a[i].pos <= queryItems[qi].pos)
//         {
//             ++tn;
//             while(i<=ir && a[i].pos <= queryItems[qi].pos)
//             {
//                 a[i].pos=tn;
//                 i++;
//             }
//         }
//         queryItems[qi].pos=tn; 
//     }
//     ++tn;
//     while(i<=ir)
//     {
//         a[i].pos=tn;
//         i++;
//     }
//     return tn;
// }
// 
// void solve(int l, int r, int il, int ir, int ql, int qr, int range) {
//     if(ql>qr)return;
//     if(l==r) {
//         for(int i=ql; i<=qr; i++) {
//             ans[queryItems[i].id] = r;
//         }
//         return;
//     }    

//     int m=(l+r)>>1;
//     static vector<int> s;
//     if(s.size()<range+1) s.resize(range+1);
//     fill(s.begin(),s.begin()+range+1,0);
//     for(int i=il;i<=ir;i++)
//         if(a[i].val<=m) 
//             s[a[i].pos]++;
//     for(int i=1;i<=range;i++)
//         s[i]+=s[i-1];
//     for(int i=ql;i<=qr;i++) queries[queryItems[i].id].k -= queryItems[i].coef * s[queryItems[i].pos];
//     int qm=divide(queryItems, ql, qr, [&](const QueryItem &x) {
//         return queries[x.id].k<=0;
//     });
//     int im=divide(a, il, ir, [&](const Item &x) {
//         return x.val <= m;
//     });
//     for(int i=ql;i<qm;i++) queries[queryItems[i].id].k += queryItems[i].coef * s[queryItems[i].pos];
//     int rangeL=reindex(il,im-1,ql,qm-1);
//     int rangeR=reindex(im,ir,qm,qr);
//     solve(l, m, il, im - 1, ql, qm - 1, rangeL);
//     solve(m + 1, r, im, ir, qm, qr, rangeR);
// };

// int main() {
//     // freopen("data.txt", "r", stdin);
// 	ios::sync_with_stdio(false);
// 	cin.tie(nullptr);

// 	int n, m;
//     cin>>n>>m;
// 	vector<int> w(n), disc;
// 	disc.reserve(n);
// 	for (int i = 0; i < n; ++i) {
// 		cin >> w[i];
// 		disc.push_back(w[i]);
// 	}
//     queries.resize(m);
//     queryItems.reserve(m*2);
// 	for (int i = 0; i < m; ++i) {
// 		cin >> queries[i].l >> queries[i].r >> queries[i].k;
// 		queries[i].id = i;
//         queryItems.push_back({queries[i].l-1, -1, i});
//         queryItems.push_back({queries[i].r, 1, i});
// 	}

// 	sort(disc.begin(), disc.end());
// 	disc.erase(unique(disc.begin(), disc.end()), disc.end());
//     a.reserve(n);
//     for (int i = 0; i < n; ++i) {
//         a.push_back({i + 1, (int)(lower_bound(disc.begin(), disc.end(), w[i]) - disc.begin() + 1)});
//     }
//     sort(a.begin(), a.end(), [](const Item &x, const Item &y) {
//         return x.pos < y.pos;
//     });
//     sort(queryItems.begin(), queryItems.end(), [](const QueryItem &x, const QueryItem &y) {
//         return x.pos < y.pos;
//     });
// 	ans.resize(m);
// 	solve(1, (int)disc.size(), 0, (int)a.size() - 1, 0, (int)queryItems.size() - 1, n);
//     for (int i = 0; i < m; ++i) ans[i] = disc[ans[i] - 1];
// 	for (int i = 0; i < m; ++i) cout << ans[i] << '\n';
// 	return 0;
// }



// 2log
// 实践上跑得比较快
struct Fenwick
{
    vector<int> s;
    void upd(int p, int v)
    {
        for(;p<(int)s.size();p+=p&-p) s[p]+=v;
    }
    int query(int p)
    {
        int res=0;
        for(;p;p-=p&-p) res+=s[p];
        return res;
    }
    int query(int l, int r)
    {
        return query(r)-query(l-1);
    }
}ds;
void solve(int l, int r, int ql, int qr) {
    if(ql>qr)return;
    if(l==r) {
        for(int i=ql; i<=qr; i++) {
            ans[queries[i].id] = a[l].val;
        }
        return;
    }    

    int m=(l+r)>>1;
    for(int i=l;i<=m;i++)
        ds.upd(a[i].pos,1);
    int qm=ql;
    static vector<Query> tmp;
    tmp.clear();
    tmp.reserve(qr-ql+1);
    for(int i=ql;i<=qr;i++)
    {
        int t=ds.query(queries[i].l,queries[i].r);
        if(queries[i].k>t)
        { 
            queries[i].k-=t;
            tmp.push_back(queries[i]);
        }
        else queries[qm++] = queries[i];
    }
    for(int i=0;i<tmp.size();i++)
        queries[qm+i] = tmp[i];
    for(int i=l;i<=m;i++)
        ds.upd(a[i].pos,-1);
    solve(l, m, ql, qm - 1);
    solve(m + 1, r,qm, qr);
};
int main() {
    // freopen("data.txt", "r", stdin);
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int n, m;
    cin>>n>>m;
    a.resize(n+1);
    ds.s.resize(n+1);
	for (int i = 1; i <= n; ++i)
    {
        a[i].pos = i;
		cin >> a[i].val;
    }
    queries.resize(m);
	for (int i = 0; i < m; ++i) {
		cin >> queries[i].l >> queries[i].r >> queries[i].k;
		queries[i].id = i;
	}
    sort(a.begin()+1, a.end(), [](const Item &x, const Item &y) {
        return x.val < y.val;
    });
	ans.resize(m);
	solve(1, n, 0, (int)queries.size() - 1);
	for (int i = 0; i < m; ++i) cout << ans[i] << '\n';
	return 0;
}



