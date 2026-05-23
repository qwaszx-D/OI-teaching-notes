#include<iostream>
#include<cstdio>
#include<cstring>
#include<vector>
#include<random>
#include<ctime>
#include<cassert>
using namespace std;
typedef unsigned long long ull;
template<unsigned mod=998244353>
struct Zp
{
    int x;
    Zp(int v=0){x=v%mod;}
    Zp operator + (const Zp& b) const {return (x + b.x) % mod;}
    Zp& operator += (const Zp& b) {x = (x + b.x) % mod; return *this;}
    Zp operator - (const Zp& b) const {return (x - b.x + mod) % mod;}
    Zp operator * (const Zp& b) const {return 1ull * x * b.x % mod;}
    friend bool operator == (const Zp& a, const Zp& b) {return a.x == b.x;}
    friend bool operator != (const Zp& a, const Zp& b) {return a.x != b.x;}
};
struct SemiGroupMax
{
    int x;
    SemiGroupMax(int v=0): x(v) {}
    friend SemiGroupMax operator + (const SemiGroupMax& a, const SemiGroupMax& b)
    {
        return max(a.x, b.x);
    }
    friend SemiGroupMax operator - (const SemiGroupMax& a, const SemiGroupMax& b)
    {
        return a==b ? SemiGroupMax(0) : a;
    }
    SemiGroupMax& operator += (const SemiGroupMax& b)
    {
        x = max(x, b.x);
        return *this;
    }
    friend bool operator == (const SemiGroupMax& a, const SemiGroupMax& b)
     {
         return a.x == b.x;
     }
};

mt19937 rnd(time(0));
int random(int l, int r)
{
    return rnd() % (r - l + 1) + l;
}

template<typename Ring, int k>
struct Matrix
{
    Ring a[k][k];
    static Matrix zero()
    {
        Matrix res;
        for(int i=0;i<k;i++)
            for(int j=0;j<k;j++)
                res.a[i][j] = 0;
        return res;
    }
    static Matrix ones()
    {
        Matrix res;
        for(int i=0;i<k;i++)
            for(int j=0;j<k;j++)
                res.a[i][j] = 1;
        return res;
    }
    static Matrix identity()
    {
        Matrix res;
        for(int i=0;i<k;i++)
            for(int j=0;j<k;j++)
                if(i==j)
                    res.a[i][j] = 1;
                else
                    res.a[i][j] = 0;
        return res;
    }

    Matrix()
    {
        for(int i=0;i<k;i++)
            for(int j=0;j<k;j++)
                a[i][j] = 0;
    }



    friend Matrix operator + (const Matrix& A, const Matrix& B)
    {
        Matrix C;
        for(int i=0;i<k;i++)
            for(int j=0;j<k;j++)
                C.a[i][j] = A.a[i][j] + B.a[i][j];
        return C;
    }
    friend Matrix operator * (const Matrix& A, const Matrix& B)
    {
        Matrix C;
        for(int i=0;i<k;i++)
            for(int t=0;t<k;t++)
                for(int j=0;j<k;j++)
                    C.a[i][j] = C.a[i][j] + A.a[i][t] * B.a[t][j];
        return C;
    }
    bool isOne() const
    {
        for(int i=0;i<k;i++)
            for(int j=0;j<k;j++)
                if(a[i][j] != (i == j))
                    return false;
        return true;
    }
    bool isZero() const
    {
        for(int i=0;i<k;i++)
            for(int j=0;j<k;j++)
                if(a[i][j] != 0)
                    return false;
        return true;
    }
    friend bool operator == (const Matrix& A, const Matrix& B)
    {
        for(int i=0;i<k;i++)
            for(int j=0;j<k;j++)
                if(A.a[i][j] != B.a[i][j])
                    return false;
        return true;
    }
};

template<typename Ring>
struct InfoSumSimp
{
    Ring s;
    InfoSumSimp(): s(0) {}
    InfoSumSimp(Ring s): s(s) {}
    friend InfoSumSimp operator + (const InfoSumSimp& a, const InfoSumSimp& b)
    {
        return {a.s + b.s};
    }
    friend bool operator == (const InfoSumSimp& a, const InfoSumSimp& b)
    {
        return a.s == b.s;
    }
};

template<typename Ring>
struct InfoSum
{
    Ring s;
    Ring len;
    InfoSum(): s(0), len(0) {}
    InfoSum(Ring s, Ring len): s(s), len(len) {}
    InfoSum(Ring x): s(x), len(1) {}
    friend InfoSum operator + (const InfoSum& a, const InfoSum& b)
    {
        return {a.s + b.s, a.len + b.len};
    }
    friend bool operator == (const InfoSum& a, const InfoSum& b)
    {
        return a.s == b.s && a.len == b.len;
    }
};
template<typename Ring, int k=2>
struct InfoMatrix
{
    Matrix<Ring, k> a;
    InfoMatrix(){}
    InfoMatrix(const Matrix<Ring, k>& a): a(a) {}
    friend InfoMatrix operator + (const InfoMatrix& A, const InfoMatrix& B)
    {
        return {A.a + B.a};
    }
    friend bool operator == (const InfoMatrix& a, const InfoMatrix& b)
    {
        return a.a == b.a;
    }
};

template<typename Ring>
struct TagPlus
{
    Ring d;
    TagPlus(Ring d = 0): d(d) {}
    InfoSum<Ring> operator () (const InfoSum<Ring>& a) const
    {
        return {a.s + a.len * d, a.len};
    }
    TagPlus operator () (const TagPlus& a) const
    {
        return {d + a.d};
    }
    bool isId() const
    {
        return d == 0;
    }
};
struct TagPlusMax
{
    int d;
    TagPlusMax(int d = 0): d(d) {}
    SemiGroupMax operator () (const SemiGroupMax& a) const
    {
        return {a.x + d};
    }
    TagPlusMax operator () (const TagPlusMax& a) const
    {
        return {d + a.d};
    }
    bool isId() const
    {
        return d == 0;
    }
};

template<typename Ring>
struct TagLinear
{
    Ring k;
    Ring b;
    TagLinear(const Ring &k = 1, const Ring &b = 0): k(k), b(b) {}
    TagLinear(const pair<Ring, Ring> &p): k(p.first), b(p.second) {}
    InfoSum<Ring> operator () (const InfoSum<Ring>& a) const
    {
        return {a.s * k + a.len * b, a.len};
    }
    TagLinear operator () (const TagLinear& a) const
    {
        return {k * a.k, k * a.b + b};
    }
    bool isId() const
    {
        return k == 1 && b == 0;
    }
};
template<typename Ring, int k=2>
struct TagMatrix
{
    Matrix<Ring,k>a;
    TagMatrix(): a(Matrix<Ring,k>::identity()) {}
    TagMatrix(const Matrix<Ring,k>& a): a(a) {}
    InfoMatrix<Ring, k> operator () (const InfoMatrix<Ring, k>& A) const
    {
        return {a * A.a};
    }
    TagMatrix operator () (const TagMatrix& B) const
    {
        return {a * B.a};
    }
    bool isId() const
    {
        return a.isOne();
    }
};

template<typename Info, typename Tag>
struct SegTree
{
    vector<Info> info;
    vector<Tag> tag;
    int n;
    SegTree(int n): n(n)
    {
        info.resize(4*n);
        tag.resize(4*n);
    }
    template<typename SeqType>
    SegTree(const vector<SeqType>& a): SegTree(a.size()-1)
    {
        build(a);
    }
    void apply(int u, const Tag& newtag)
    {
        info[u] = newtag(info[u]);
        tag[u] = newtag(tag[u]);
    }
    void pushdown(int u)
    {
        if (!tag[u].isId())
        {
            apply(u << 1, tag[u]);
            apply(u << 1 | 1, tag[u]);
            tag[u] = Tag();
        }
    }
    template<typename SeqType>
    void build(int u, int l, int r, const vector<SeqType>& a)
    {
        if (l == r)
        {
            info[u] = Info(a[l]);
            return;
        }
        tag[u]=Tag();
        int mid = (l + r) >> 1;
        build(u << 1, l, mid, a);
        build(u << 1 | 1, mid + 1, r, a);
        info[u] = info[u << 1] + info[u << 1 | 1];
    }
    Info query(int u, int l, int r, int lq, int rq)
    {
        if (lq == l && r == rq)
            return info[u];
        int mid = (l + r) >> 1;
        pushdown(u);
        if (rq <= mid)
            return query(u << 1, l, mid, lq, rq);
        else if(lq > mid)
            return query(u << 1 | 1, mid + 1, r, lq, rq);
        else
            return query(u << 1, l, mid, lq, mid) + query(u << 1 | 1, mid + 1, r, mid + 1, rq);
    }
    void update(int u, int l, int r, int lq, int rq, const Tag& newtag)
    {
        if(lq == l && r == rq)
        {
            apply(u, newtag);
            return;
        }
        int mid = (l + r) >> 1;
        pushdown(u);
        if (rq <= mid)
            update(u << 1, l, mid, lq, rq, newtag);
        else if (lq > mid)
            update(u << 1 | 1, mid + 1, r, lq, rq, newtag);
        else
        {
            update(u << 1, l, mid, lq, mid, newtag);
            update(u << 1 | 1, mid + 1, r, mid + 1, rq, newtag);
        }
        info[u] = info[u << 1] + info[u << 1 | 1];
    }
    Info query(int lq, int rq)
    {
        return query(1, 1, n, lq, rq);
    }
    void update(int lq, int rq, const Tag &d)
    {
        update(1, 1, n, lq, rq, d);
    }
    template<typename SeqType>
    void build(const vector<SeqType>& a)
    {
        build(1, 1, n, a);
    }
};

template<typename Info>
struct SegTreeSingleUpdate
{
    vector<Info> info;
    int n;
    SegTreeSingleUpdate(int n): n(n)
    {
        info.resize(4*n);
    }
    template<typename SeqType>
    SegTreeSingleUpdate(const vector<SeqType>& a): SegTreeSingleUpdate(a.size()-1)
    {
        build(a);
    }
    template<typename SeqType>
    void build(int u, int l, int r, const vector<SeqType>& a)
    {
        if (l == r)
        {
            info[u] = Info(a[l]);
            return;
        }
        int mid = (l + r) >> 1;
        build(u << 1, l, mid, a);
        build(u << 1 | 1, mid + 1, r, a);
        info[u] = info[u << 1] + info[u << 1 | 1];
    }
    Info query(int u, int l, int r, int lq, int rq)
    {
        if (lq == l && r == rq)
            return info[u];
        int mid = (l + r) >> 1;
        if (rq <= mid)
            return query(u << 1, l, mid, lq, rq);
        else if(lq > mid)
            return query(u << 1 | 1, mid + 1, r, lq, rq);
        else
            return query(u << 1, l, mid, lq, mid) + query(u << 1 | 1, mid + 1, r, mid + 1, rq);
    }
    void update(int u, int l, int r, int q, const Info& val)
    {
        if(l == r)
        {
            info[u] = val;
            return;
        }
        int mid = (l + r) >> 1;
        if (q <= mid)
            update(u << 1, l, mid, q, val);
        else
            update(u << 1 | 1, mid + 1, r, q, val);
        info[u] = info[u << 1] + info[u << 1 | 1];
    }
    Info query(int lq, int rq)
    {
        return query(1, 1, n, lq, rq);
    }
    void update(int q, const Info &d)
    {
        update(1, 1, n, q, d);
    }
    template<typename SeqType>
    void build(const vector<SeqType>& a)
    {
        build(1, 1, n, a);
    }
};

template<typename Info, typename Tag>
struct SegTreeZKW
{
    vector<Info> info;
    vector<Tag> tag;
    int n;
    SegTreeZKW(int n): n(n)
    {
        info.resize(2*n+2);
        tag.resize(2*n+2);
    }
    template<typename SeqType>
    SegTreeZKW(const vector<SeqType>& a): SegTreeZKW(a.size()-1)
    {
        build(a);
    }
    void apply(int u, const Tag& newtag)
    {
        info[u] = newtag(info[u]);
        tag[u] = newtag(tag[u]);
    }
    void pushdown(int u)
    {
        if (!tag[u].isId())
        {
            apply(u << 1, tag[u]);
            apply(u << 1 | 1, tag[u]);
            tag[u] = Tag();
        }
    }
    template<typename SeqType>
    void build(const vector<SeqType>& a)
    {
        for(int i=1;i<=n;i++)
            info[n + i] = Info(a[i]);
        for(int i=n-1;i>=1;i--)
            info[i] = info[i << 1] + info[i << 1 | 1];
    }
    Info query(int lq, int rq)
    {
        lq += n;
        rq += n+1;
        for(int i=__lg(n)+1;i>=1;i--)
        {
            pushdown(lq >> i);
            pushdown(rq >> i);
        }
        Info resl, resr;
        while(lq < rq)
        {
            if (lq & 1)
                resl = resl + info[lq++];
            if (rq & 1)
                resr = info[--rq] + resr;
            lq >>= 1;
            rq >>= 1;
        }
        return resl+resr;
    }
    void update(int lq, int rq, const Tag& newtag)
    {
        lq += n;
        rq += n+1;
        for(int i=__lg(n)+1;i>=1;i--)
        {
            pushdown(lq >> i);
            pushdown(rq >> i);
        }
        int u=0, v=0;
        while(lq < rq)
        {
            if (lq & 1)
            {
                u=lq;
                apply(lq++, newtag);
                
            }
            if (rq & 1)
            {
                v=rq;    
                apply(--rq, newtag);
            }
            do{u>>=1; info[u]=info[u << 1] + info[u << 1 | 1];}while(lq==rq && u);
            do{v>>=1; info[v]=info[v << 1] + info[v << 1 | 1];}while(lq==rq && v);
            lq >>= 1;
            rq >>= 1;
        }
    }
};
template<typename Info>
struct SegTreeZKWSingleUpdate
{
    vector<Info> info;
    int n;
    SegTreeZKWSingleUpdate(int n): n(n)
    {
        info.resize(2*n+2);
    }
    template<typename SeqType>
    SegTreeZKWSingleUpdate(const vector<SeqType>& a): SegTreeZKWSingleUpdate(a.size()-1)
    {
        build(a);
    }
    template<typename SeqType>
    void build(const vector<SeqType>& a)
    {
        for(int i=1;i<=n;i++)
            info[n + i] = Info(a[i]);
        for(int i=n-1;i>=1;i--)
            info[i] = info[i << 1] + info[i << 1 | 1];
    }
    Info query(int lq, int rq)
    {
        lq += n;
        rq += n+1;
        Info resl, resr;
        while(lq < rq)
        {
            if (lq & 1)
                resl = resl + info[lq++];
            if (rq & 1)
                resr = info[--rq] + resr;
            lq >>= 1;
            rq >>= 1;
        }
        return resl+resr;
    }
    void update(int q, const Info& val)
    {
        int u = q + n;
        info[u] = val;
        while(u > 1)
        {
            u >>= 1;
            info[u] = info[u << 1] + info[u << 1 | 1];
        }
    }
};
template<typename CommRing>
struct BIT
{
    vector<CommRing> bit;
    int n;
    BIT(int n): n(n)
    {
        bit.resize(n + 1);
    }
    template<typename SeqType>
    BIT(const vector<SeqType>& a): BIT(a.size()-1)
    {
        build(a);
    }
    template<typename SeqType>
    void build(const vector<SeqType>& a)
    {
        for(int i=1;i<=n;i++)
            bit[i]=CommRing(a[i]);
        for(int i=1;i<=n;i<<=1)
            for(int j=i;j+i<=n;j+=i<<1)
                bit[j+i] += bit[j];
    }
    void update(int x, CommRing d)
    {
        for(;x<=n;x+=x&-x)
            bit[x] += d;
    }
    CommRing query(int x)
    {
        CommRing res=0;
        for(;x;x-=x&-x)
            res += bit[x];
        return res;
    }
    CommRing query(int l, int r)
    {
        return query(r) - query(l - 1);
    }
};
template<typename Info, typename DS, typename SeqType, typename QType, bool singleUpdate>
tuple<vector<Info>,double,double> runTest(const vector<SeqType>& a, const vector<QType>& queries)
{
    clock_t begt=clock();
    DS ds(a);
    double buildDuration = (clock() - begt) * 1.0 / CLOCKS_PER_SEC;
    vector<Info>ans;
    begt=clock();
    for(auto& q: queries)
    {
        if(q.op == 1)
        {
            if constexpr (singleUpdate)
                ds.update(q.l, q.w);
            else
                ds.update(q.l, q.r, q.w);
        }
        else ans.emplace_back(ds.query(q.l, q.r));
    }
    double queryDuration = (clock() - begt) * 1.0 / CLOCKS_PER_SEC;
    return make_tuple(ans, buildDuration, queryDuration);
}


template<typename T>
struct randomGenerator
{
    T operator()()
    {
        return T(rnd());
    }
};

template<typename T>
T genRandomElement()
{
    return randomGenerator<T>()();
}


template<typename Ring, int k>
struct randomGenerator<Matrix<Ring, k>>
{
    Matrix<Ring, k> operator()()
    {
        Matrix<Ring, k> res;
        for(int i=0;i<k;i++)
            for(int j=0;j<k;j++)
                res.a[i][j] = genRandomElement<Ring>();
        return res;
    }
};

template<>
struct randomGenerator<SemiGroupMax>
{
    SemiGroupMax operator()()
    {
        return SemiGroupMax(rnd()%1000);
    }
};


template<typename Info>
struct QItemSingleUpdate
{
    int op;
    int l, r;
    Info w;
};

template<typename Tag>
struct QItem
{
    int op;
    int l, r;
    Tag w;
};

template<typename SeqType, typename Info>vector<QItemSingleUpdate<Info>> genRandomQuerySingleUpdateSequence(int n, int m)
{
    vector<QItemSingleUpdate<Info>> queries(m);
    for(auto& q: queries)
    {
        q.op = random(1,2);
        if (q.op == 1)
        {
            q.l = random(1, n);
            q.w = Info(genRandomElement<SeqType>());
        }
        else
        {
            q.l = random(1, n);
            q.r = random(1, n);
            if (q.l > q.r)
                swap(q.l, q.r);
        }
    }
    return queries;
}
template<typename SeqType, typename Tag>vector<QItem<Tag>> genRandomQuerySequence(int n, int m)
{
    vector<QItem<Tag>> queries(m);
    for(auto& q: queries)
    {
        q.op = random(1, 2);
        q.l = random(1, n);
        q.r = random(1, n);
        if (q.l > q.r)
            swap(q.l, q.r);
        if (q.op == 1)
            q.w = Tag(genRandomElement<SeqType>());
    }
    return queries;
}

template<typename T>bool operator ==(const vector<T>& a, const vector<T>& b)
{
    if (a.size() != b.size())
        return false;
    for(int i=0;i<a.size();i++)
        if (!(a[i] == b[i]))
            return false;
    return true;
}

template<typename SeqType, typename Info, typename Tag>void testRangeUpdate(const vector<int>&testN)
{
    cout<<"信息类型："<<typeid(Info).name()<<"\n";
    cout<<"区间修改类型："<<typeid(Tag).name()<<"\n";

    int test_num = testN.size();
    using QType = QItem<Tag>;
    vector<vector<QType>> allQueries(test_num);
    vector<vector<SeqType>> allAs(test_num);
    for(int i=0;i<test_num;i++)
    {
        int n=testN[i], m=testN[i];
        vector<SeqType> a(n+1);
        for(int j=1;j<=n;j++)
            a[j] = genRandomElement<SeqType>();
        allAs[i] = a;
        allQueries[i] = genRandomQuerySequence<SeqType, Tag>(n, m);
        cout<<"生成询问规模："<<m<<"\n";
    }
    cout<<"n:\t";
    for(int n: testN)
        cout<<n<<'\t';
    cout<<'\n';
    cout<<"递归线段树:\t";
    vector<tuple<vector<Info>, double, double>> resSegs(test_num);
    for(int i=0;i<test_num;i++)
    {
        resSegs[i] = runTest<Info, SegTree<Info, Tag>, SeqType, QItem<Tag>, false>(allAs[i], allQueries[i]);
        cout<<'('<<get<1>(resSegs[i])<<", "<<get<2>(resSegs[i])<<')'<<'\t';
    }
    cout<<"\n";

    cout<<"ZKW 线段树:\t";
    vector<tuple<vector<Info>, double, double>> resSegZKWs(test_num);
    for(int i=0;i<test_num;i++)
    {
        resSegZKWs[i] = runTest<Info, SegTreeZKW<Info, Tag>, SeqType, QItem<Tag>, false>(allAs[i], allQueries[i]);
        cout<<'('<<get<1>(resSegZKWs[i])<<", "<<get<2>(resSegZKWs[i])<<')'<<'\t';
    }
    for(int i=0;i<test_num;i++)
        assert(get<0>(resSegs[i]) == get<0>(resSegZKWs[i]));
    cout<<"\n";
    cout<<"-------------------------------\n";
}

template<typename SeqType, typename Info>void testSingleUpdate(const vector<int>&testN)
{
    cout<<"信息类型："<<typeid(Info).name()<<"\n";
    cout<<"单点修改"<<"\n";
    int test_num = testN.size();
    using QType = QItemSingleUpdate<Info>;
    vector<vector<QType>> allQueries(test_num);
    vector<vector<SeqType>> allAs(test_num);
    for(int i=0;i<test_num;i++)
    {
        int n=testN[i], m=testN[i];
        vector<SeqType> a(n+1);
        for(int j=1;j<=n;j++)
            a[j] = genRandomElement<SeqType>();
        allAs[i] = a;
        allQueries[i] = genRandomQuerySingleUpdateSequence<SeqType, Info>(n, m);
        cout<<"生成询问规模："<<m<<"\n";
    }
    cout<<"n:\t";
    for(int n: testN)
        cout<<n<<'\t';
    cout<<'\n';
    cout<<"递归线段树:\t";
    vector<tuple<vector<Info>, double, double>> resSegs(test_num);
    for(int i=0;i<test_num;i++)
    {
        resSegs[i] = runTest<Info, SegTreeSingleUpdate<Info>, SeqType, QItemSingleUpdate<Info>, true>(allAs[i], allQueries[i]);
        cout<<'('<<get<1>(resSegs[i])<<", "<<get<2>(resSegs[i])<<')'<<'\t';
    }
    cout<<"\n";
    cout<<"ZKW 线段树:\t";
    vector<tuple<vector<Info>, double, double>> resSegZKWs(test_num);
    for(int i=0;i<test_num;i++)
    {
        resSegZKWs[i] = runTest<Info, SegTreeZKWSingleUpdate<Info>, SeqType, QItemSingleUpdate<Info>, true>(allAs[i], allQueries[i]);
        cout<<'('<<get<1>(resSegZKWs[i])<<", "<<get<2>(resSegZKWs[i])<<')'<<'\t';
    }
    cout<<"\n";
    // for(int i=0;i<test_num;i++)
    // {
    //     assert(get<0>(resSegs[i]) == get<0>(resSegZKWs[i]));
    // }
    if constexpr (is_same_v<Info, ull> || is_same_v<Info, Zp<>> || is_same_v<Info, SemiGroupMax>)
    {
        cout<<"树状数组:\t";
        vector<tuple<vector<Info>, double, double>> resBITs(test_num);
        for(int i=0;i<test_num;i++)
        {
            resBITs[i] = runTest<Info, BIT<Info>, SeqType, QItemSingleUpdate<Info>, true>(allAs[i], allQueries[i]);
            cout<<'('<<get<1>(resBITs[i])<<", "<<get<2>(resBITs[i])<<')'<<'\t';
        }
        cout<<"\n";
    }
    cout<<"-------------------------------\n";
}


int main()
{
    freopen("test_result.txt","w",stdout);
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    vector<int> testN = {1000, 100000, 1000000};

    testSingleUpdate<unsigned char, SemiGroupMax>(testN);
    testSingleUpdate<ull, ull>(testN);
    testSingleUpdate<Zp<>, Zp<>>(testN);
    testSingleUpdate<Matrix<Zp<>,2>, InfoMatrix<Zp<>,2>>(testN);
    testSingleUpdate<Matrix<Zp<>,3>, InfoMatrix<Zp<>,3>>(testN);
    testSingleUpdate<Matrix<Zp<>,4>, InfoMatrix<Zp<>,4>>(testN);

    testRangeUpdate<unsigned char, SemiGroupMax, TagPlusMax>(testN);
    testRangeUpdate<ull, InfoSum<ull>, TagPlus<ull>>(testN);
    testRangeUpdate<ull, InfoSum<ull>, TagLinear<ull>>(testN);
    testRangeUpdate<Matrix<Zp<>,2>, InfoMatrix<Zp<>,2>, TagMatrix<Zp<>,2>>(testN);

}