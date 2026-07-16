#include<bits/stdc++.h>
using namespace std;
const int INF=1e9;
struct SuffixTree
{
    int newNode(int start, int len)
    {
        a.emplace_back(start, len);
        return a.size()-1;
    }
    void print()
    {
        cerr<<"cur="<<cur<<" len="<<len<<endl;
        function<void(int, int, int)>dfs;
        dfs = [&](int u, int v, int c)
        {
            cerr<<u<<" "<<"fa="<<v<<" c="<<c<<endl;
            cerr<<"start="<<a[u].start<<endl;
            cerr<<"len="<<a[u].len<<endl;
            cerr<<"link="<<a[u].link<<endl;
            cerr<<"cnt="<<a[u].cnt<<endl;
            for(int i=0;i<26;i++)
            {
                int v=a[u].ch[i];
                if(v)dfs(v, u, i);
            }
        };
        dfs(1, 0, 0);
    }
    void ins(int c)
    {
        // cerr<<"add "<<c<<endl;
        s.push_back(c);
        ++len;
        int n=s.size();
        for(int lst=0;len;)
        {
            // 计算走 len 步到达的位置
            int u=a[cur].ch[s[n-len]];
            while(len > a[u].len)
            {
                cur = u;
                len -= a[u].len;
                u = a[cur].ch[s[n-len]];
            }
            // 到达位置在 (cur,u] 的边上
            if(!u)
            {
                // 处在非叶结点上，添加新叶子
                if(lst)
                    a[lst].link=cur;
                lst=0;
                assert(len==1);
                u=newNode(n-1,INF);
                a[cur].ch[c]=u;
                a[u].cnt++;
                pos.emplace_back(u);
            }
            else
            {
                int tc=s[a[u].start+len-1];
                if(tc==c)
                {
                    // 已经存在
                    if(lst)
                        a[lst].link=cur;


                    lst=0;
                    break;
                }

                else
                {
                    // 处在边上，分裂
                    int v=newNode(a[u].start,len-1);
                    a[cur].ch[s[a[v].start]]=v;
                    a[v].ch[tc]=u;
                    a[u].len -= len-1;
                    a[u].start += len-1;

                    u = a[v].ch[c]=newNode(n-1,INF);
                    a[u].cnt++;
                    pos.emplace_back(u);

                    if(lst)
                        a[lst].link=v;
                    lst=v;
                }
            }
            // 移动到 k+1 所在的位置
            if(cur==1) --len;
            else cur=a[cur].link;
        }
        // print();
    }
    void finish()
    {
        // 从隐式后缀树得到后缀树
        n=s.size();
        for(auto u:pos)
            a[u].len=n-a[u].start;
        for(int lst=0;len;)
        {
            // 计算走 len 步到达的位置
            int u=a[cur].ch[s[n-len]];
            while(len > a[u].len)
            {
                cur = u;
                len -= a[u].len;
                u = a[cur].ch[s[n-len]];
            }
            // cerr<<cur<<" "<<len<<" "<<u<<endl;
            // 到达位置在 (cur,u] 的边上
            assert(u);
            if(a[u].len==len)
            {
                ++a[u].cnt;
                pos.emplace_back(u);
                if(lst)a[lst].link=u;
                lst=0;
            }
            else
            {
                // 处在边上，分裂
                int tc=s[a[u].start+len];
                int v=newNode(a[u].start,len);
                a[cur].ch[s[a[v].start]]=v;
                a[v].ch[tc]=u;
                ++a[v].cnt;
                pos.emplace_back(v);
                a[u].len -= len;
                a[u].start += len;
                
                if(lst)a[lst].link=v;
                lst=v;
            }
            // 移动到 k+1 所在的位置
            if(cur==1) --len;
            else cur=a[cur].link;
        }
    }
    SuffixTree(string st): n(0), cur(1), len(0), a(2)
    {
        a[0].len=INF;
        s.reserve(st.size());
        a.reserve(st.size()*2);
        pos.reserve(st.size());
        for(auto c:st)
            ins(c-'a');
        finish();
    }
    struct Node
    {
        int ch[26];
        int start, len;
        int link;
        int cnt;
        Node(): ch{0}, start(0), len(0), link(0), cnt(0) {}
        Node(int start, int len): ch{0}, start(start), len(len), link(0), cnt(0) {}
    };
    int n;
    int cur, len;
    vector<Node>a;
    vector<int>s;
    vector<int>pos;

    long long solve()
    {
        long long ans=0;
        function<void(int,int)>dfs;
        dfs = [&](int u, int len)
        {
            
            len+=a[u].len;
            for(int i=0;i<26;i++)
            {
                int v=a[u].ch[i];
                if(v)
                {
                    dfs(v,len);
                    a[u].cnt+=a[v].cnt;
                }
            }
            // cerr<<u<<" "<<len<<" "<<a[u].cnt<<endl;
            if(a[u].cnt>=2)
                ans=max(ans,1ll*a[u].cnt*len);
        };
        dfs(1,0);
        return ans;
    }
};

int main()
{
    // freopen("data.txt","r",stdin);
    ios::sync_with_stdio(false);
    cin.tie(0);
    string st;
    cin>>st;
    SuffixTree a(st);
    cout<<a.solve()<<endl;
}