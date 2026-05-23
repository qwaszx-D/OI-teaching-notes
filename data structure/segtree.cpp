#include<iostream>
#include<cstdio>
#include<cstring>
#include<vector>
#include<random>
#include<array>
#include<ctime>
#include<algorithm>
#include<cassert>
using namespace std;
typedef unsigned long long ull;
// 模 m 运算
struct Zm
{
    static unsigned mod;
    int x;
    Zm(int v=0){x=v%mod;}
    Zm operator + (const Zm& b) const {return (x + b.x) % mod;}
    Zm& operator += (const Zm& b) {x = (x + b.x) % mod; return *this;}
    Zm operator - (const Zm& b) const {return (x - b.x + mod) % mod;}
    Zm operator * (const Zm& b) const {return 1ull * x * b.x % mod;}
    friend bool operator == (const Zm& a, const Zm& b) {return a.x == b.x;}
    friend bool operator != (const Zm& a, const Zm& b) {return a.x != b.x;}
};
unsigned Zm::mod;

// 模常数模数 p 的运算
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

template<typename Ring, int k=2>
struct InfoVector
{
    array<Ring, k> a;
    InfoVector(){}
    InfoVector(const array<Ring, k>& a): a(a) {}
    friend InfoVector operator + (const InfoVector& A, const InfoVector& B)
    {
        InfoVector ans;
        for(int i=0;i<k;i++)
            ans.a[i]=A.a[i]+B.a[i];
        return ans;
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

unsigned char charRemap[128];
bool connectLine[5][5], connectCol[5][5];
struct Info3300
{
    vector<unsigned char> sl,sr;
    vector<unsigned char>idl, idr;
    vector<unsigned char>hasBuilding;
    static int m;
    int cnt;
    int innerCnt;
    // void print()const
    // {
    //     cerr<<"sl="<<sl<<" sr="<<sr<<" cnt="<<cnt<<" innerCnt="<<innerCnt<<endl;
    //     cerr<<"idl: ";
    //     for(int i=0;i<m;i++)
    //         cerr<<(int)idl[i]<<" ";
    //     cerr<<endl;
    //     cerr<<"idr: ";
    //     for(int i=0;i<m;i++)
    //         cerr<<(int)idr[i]<<" ";
    //     cerr<<endl<<"----------------------------"<<endl;
    // }
    Info3300():idl(m),idr(m),cnt(0),innerCnt(0){}
    Info3300(const vector<unsigned char>& s):Info3300()
    {
        sl=sr=s;
        int curid=0;
        idl[0]=idr[0]=curid;
        hasBuilding.push_back(s[0]==charRemap['O']);
        for(int i=1;i<m;i++)
        {
            if(!connectLine[s[i-1]][s[i]])
            {
                ++curid;
                hasBuilding.push_back(0);
            }
            idl[i]=idr[i]=curid;
            hasBuilding[curid]|=(s[i]==charRemap['O']);
        }
        for(int i=0;i<=curid;i++)
            cnt += hasBuilding[i];
    }
    friend Info3300 operator + (const Info3300& A, const Info3300& B)
    {
        Info3300 ans;
        ans.sl=A.sl;
        ans.sr=B.sr;
        ans.innerCnt=A.innerCnt+B.innerCnt;
        struct DSU
        {
            vector<int>fa;
            DSU(int n):fa(n){for(int i=0;i<n;i++)fa[i]=i;}
            int find(int x)
            {
                if(fa[x]!=x) fa[x]=find(fa[x]);
                return fa[x];
            }
            void merge(int x, int y)
            {
                x=find(x), y=find(y);
                if(x!=y) fa[y]=x;
            }
        };
        int ccA=A.hasBuilding.size(), ccB=B.hasBuilding.size();
        DSU dsu(ccA+ccB);
        vector<unsigned char>Aidl(A.idl),Aidr(A.idr),Bidl(B.idl),Bidr(B.idr);
        for(int i=0;i<m;i++)
            Bidl[i]+=ccA,Bidr[i]+=ccA;
        for(int i=0;i<m;i++)
            if(connectCol[A.sr[i]][B.sl[i]])
                dsu.merge(Aidr[i],Bidl[i]);
        // for(int i=0;i<m;i++)
        //     cerr<<dsu.find(Aidl[i])<<" ";
        // cerr<<endl;
        // for(int i=0;i<m;i++)
        //     cerr<<dsu.find(Bidr[i])<<" ";
        // cerr<<endl;
        vector<unsigned char>hasBuilding(ccA+ccB),inBoundary(ccA+ccB);
        for(int i=0;i<m;i++)
        {
            hasBuilding[dsu.find(Aidl[i])]|=A.hasBuilding[A.idl[i]];
            hasBuilding[dsu.find(Aidr[i])]|=A.hasBuilding[A.idr[i]];
            hasBuilding[dsu.find(Bidl[i])]|=B.hasBuilding[B.idl[i]];
            hasBuilding[dsu.find(Bidr[i])]|=B.hasBuilding[B.idr[i]];
        }
        for(int i=0;i<m;i++)
        {
            inBoundary[dsu.find(Aidl[i])]|=1;
            inBoundary[dsu.find(Bidr[i])]|=1;
        }
        for(int i=0;i<ccA+ccB;i++)
            if(!inBoundary[i]&&hasBuilding[i])
                ans.innerCnt++;
        vector<unsigned char>idmap(ccA+ccB),idset;
        for(int i=0;i<ccA+ccB;i++)
            if(inBoundary[i])
                idset.push_back(i);
        for(int i=0;i<idset.size();i++)
            idmap[idset[i]]=i;
        for(int i=0;i<m;i++)
        {
            ans.idl[i]=idmap[dsu.find(Aidl[i])];
            ans.idr[i]=idmap[dsu.find(Bidr[i])];
        }
        ans.cnt=ans.innerCnt;
        ans.hasBuilding.resize(idset.size());
        for(int i=0;i<idset.size();i++)
        {
            ans.hasBuilding[i]=hasBuilding[idset[i]];
            if(hasBuilding[idset[i]])
                ans.cnt++;
        }
        return ans;
    }
};
int Info3300::m;


// P7453
const unsigned mod=998244353;
struct Info
{
    array<Zp<mod>, 4> a;
    Info(){}
    Info(const array<Zp<mod>, 3>& a_) 
    {
        for(int i=0;i<3;i++)
            a[i]=a_[i];
        a[3]=1;
    }
    friend Info operator + (const Info& A, const Info& B)
    {
        Info ans;
        for(int i=0;i<4;i++)
            ans.a[i]=A.a[i]+B.a[i];
        return ans;
    }
};
struct Tag
{
    Zp<mod> a[3][4];
    bool isid;
    Tag():isid(1)
    {
        for(int i=0;i<3;i++)
            a[i][i]=1;
    }
    Tag(int type, Zp<mod> c=0): Tag(){
        isid=0;
        switch (type)
        {
            case 0:
            {
                isid=1;
                for(int i=0;i<3;i++)
                    a[i][i]=0;
                break;
            }
            case 1:
            {
                a[0][1]=1;
                break;
            }
            case 2:
            {
                a[1][2]=1;
                break;
            }
            case 3:
            {
                a[2][0]=1;
                break;
            }
            case 4:
            {
                a[0][3]=c;
                break;
            }
            case 5:
            {
                a[1][1]=c;
                break;
            }
            case 6:
            {
                a[2][3]=c;
                a[2][2]=0;
                break;
            }
        }
    }
    Info operator () (const Info& s) const
    {
        Info ans;
        for(int i=0;i<3;i++)
            for(int j=0;j<4;j++)
                ans.a[i] += a[i][j] * s.a[j];
        ans.a[3]=s.a[3];
        return ans;
    }
    Tag operator () (const Tag& b) const
    {
        if(b.isid) return *this;
        Tag ans(0);
        ans.isid=0;
        for(int i=0;i<3;i++)
        {
            for(int k=0;k<3;k++)
                for(int j=0;j<4;j++)
                    ans.a[i][j] += a[i][k] * b.a[k][j];
            ans.a[i][3] += a[i][3];
        }
        return ans;
    }
    bool isId() const
    {
        return isid;
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
        if (!tag[u].isId())  //这一行不必要，但是一般加上会快
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
    template<typename SeqType>
    void update(int u, int l, int r, int q, const SeqType& val)
    {
        if (l == r)
        {
            info[u] = Info(val);
            return;
        }
        int mid = (l + r) >> 1;
        pushdown(u);
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
struct SegTreeSingle
{
    vector<Info> info;
    int n;
    SegTreeSingle(int n): n(n)
    {
        info.resize(4*n);
    }
    template<typename SeqType>
    SegTreeSingle(const vector<SeqType>& a): SegTreeSingle(a.size()-1)
    {
        build(a);
    }
    template<typename SeqType>
    void build(int u, int l, int r, const vector<SeqType>& a)
    {
        if (l == r)
        {
            info[u] = Info(a[l]);
            // cerr<<l<<" "<<r<<endl;
            // info[u].print();
            return;
        }
        int mid = (l + r) >> 1;
        build(u << 1, l, mid, a);
        build(u << 1 | 1, mid + 1, r, a);
        info[u] = info[u << 1] + info[u << 1 | 1];
        // cerr<<l<<" "<<r<<endl;
        // info[u].print();
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
    template<typename SeqType>
    void update(int u, int l, int r, int q, const SeqType& val)
    {
        if (l == r)
        {
            info[u] = Info(val);
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
    template<typename SeqType>
    void build(const vector<SeqType>& a)
    {
        build(1, 1, n, a);
    }
    template<typename SeqType>
    void update(int q, const SeqType& val)
    {
        update(1, 1, n, q, val);
    }
};


//以查询区间 [l,r] 内最左的满足 s[l,i]>=x 的 i 的位置为例
// int query(int u, int l, int r, int lq, int rq, Info& preinfo, int x)
// {
//     if(l==lq&&r==rq)
//     {
//         if(preinfo.s+info[u].s<x)
//         {
//             preinfo = preinfo + info[u];
//             return -1;
//         }
//         //否则，说明答案就在此区间内
//     }
//     if(l==r)
//         return l;
//     pushdown(u);
//     int mid=(l+r)>>1;
//     if(rq<=mid)
//         return query(u<<1, l, mid, lq, rq, preinfo, x);
//     else if(lq>mid)
//         return query(u<<1|1, mid+1, r, lq, rq, preinfo, x);
//     else
//     {
//         int ans=query(u<<1, l, mid, lq, mid, preinfo, x);
//         if(ans!=-1) return ans;
//         else return query(u<<1|1, mid+1, r, mid+1, rq, preinfo, x);
//     }
// }



// template<typename Ring, int k=2>
// struct TagMatrix
// {
//     Matrix<Ring,k>a;
//     TagMatrix(): a(Matrix<Ring,k>::identity()) {}
//     TagMatrix(const Matrix<Ring,k>& a): a(a) {}
//     InfoMatrix<Ring, k> operator () (const InfoMatrix<Ring, k>& A) const
//     {
//         return {a * A.a};
//     }
//     TagMatrix operator () (const TagMatrix& B) const
//     {
//         return {a * B.a};
//     }
//     bool isId() const
//     {
//         return a.isOne();
//     }
// };



// // 不需要 build 了，可以使用 update 来替代 build
// // 假设不存在的结点编号为 0, 使用 newNode(l,r) 来创建新结点，初始化为 [l,r] 的默认信息
// // 大多数情况下，默认信息都是 0，此时 newNode 和 pushdown 都不需要传入 l,r。
// int newNode(int l, int r)
// {
//     ++nodeCnt;
//     ch[nodeCnt][0] = ch[nodeCnt][1] = 0;
//     info[nodeCnt] = defaultInfo(l, r);
//     tag[nodeCnt] = Tag();
// }
// void pushdown(int u, int l, int r)
// {
//     int mid = (l + r) >> 1;
//     if(!tag[u].isId())
//     {
//         if(!ch[u][0]) ch[u][0]=newNode(l, mid);
//         if(!ch[u][1]) ch[u][1]=newNode(mid + 1, r);
//         apply(ch[u][0], tag[u]);
//         apply(ch[u][1], tag[u]);
//         tag[u] = Tag();
//     }
// }
// Info query(int u, int l, int r, int lq, int rq)
// {
//     if(!u) return defaultInfo(lq,rq);
//     if (lq == l && r == rq)
//         return info[u];
//     int mid = (l + r) >> 1;
//     pushdown(u, l, r);
//     if (rq <= mid)
//         return query(ch[u][0], l, mid, lq, rq);
//     else if(lq > mid)
//         return query(ch[u][1], mid + 1, r, lq, rq);
//     else
//         return query(ch[u][0], l, mid, lq, mid) + query(ch[u][1], mid + 1, r, mid + 1, rq);
// }
// void update(int &u, int l, int r, int lq, int rq, const Tag& newtag)
// {
//     if(!u) u=newNode(l,r);
//     if(lq == l && r == rq)
//     {
//         apply(u, newtag);
//         return;
//     }
//     int mid = (l + r) >> 1;
//     pushdown(u, l, r);
//     if (rq <= mid)
//         update(ch[u][0], l, mid, lq, rq, newtag);
//     else if (lq > mid)
//         update(ch[u][1], mid + 1, r, lq, rq, newtag);
//     else
//     {
//         update(ch[u][0], l, mid, lq, mid, newtag);
//         update(ch[u][1], mid + 1, r, mid + 1, rq, newtag);
//     }
//     info[u] = (ch[u][0] ? info[ch[u][0]] : defaultInfo(l,mid)) + (ch[u][1] ? info[ch[u][1]] : defaultInfo(mid+1,r));
//     // 如果 defaultInfo 的代价比较大，那么也可以选择在这里把不存在的子节点建出来。这样做会让空间变为两倍。
// }

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    // freopen("data.txt","r",stdin);
    charRemap['-']=0, charRemap['|']=1, charRemap['+']=2, charRemap['O']=3, charRemap['.']=4;
    connectLine[0][0]=connectLine[0][2]=connectLine[2][0]=connectLine[2][2]=1;
    connectLine[0][3]=connectLine[3][0]=connectLine[2][3]=connectLine[3][2]=1;
    connectLine[3][3]=1;
    connectCol[1][1]=connectCol[1][2]=connectCol[2][1]=connectCol[2][2]=1;
    connectCol[1][3]=connectCol[3][1]=connectCol[2][3]=connectCol[3][2]=1;
    connectCol[3][3]=1;

    unsigned n,q,m;
    cin>>n>>m;
    Info3300::m=m;
    vector<string> a(n+1);
    vector<vector<unsigned char>>remappedA(n+1, vector<unsigned char>(m));
    for(int i=1;i<=n;i++)
    {
        cin>>a[i];
        for(int j=0;j<m;j++)
            remappedA[i][j]=charRemap[a[i][j]];
    }
    SegTreeSingle<Info3300> seg(remappedA);
    cin>>q;
    while(q--)
    {
        string op;
        int l, r;
        cin>>op>>l>>r;
        if(op=="C")
        {
            string c;
            cin>>c;
            remappedA[l][r-1]=charRemap[c[0]];
            seg.update(l,remappedA[l]);
        }
        else
        {
            auto ans=seg.query(l,r);
            cout<<ans.cnt<<"\n";
        }
    }

     return 0;

}