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
struct DSU
{
    vector<int> fa;
    DSU(int n) : fa(n)
    {
        for (int i = 0; i < n; i++)
            fa[i] = i;
    }
    int find(int x)
    {
        return x == fa[x] ? x : fa[x] = find(fa[x]);
    }
    void merge(int x, int y)
    {
        x = find(x);
        y = find(y);
        if (x != y)
            fa[y] = x;
    }
};
const int dx[]={-1,0,1,0},dy[]={0,1,0,-1};
bool connectLine[128][128], connectCol[128][128];
int main()
{
    connectLine['-']['-']=connectLine['-']['+']=connectLine['+']['-']=connectLine['+']['+']=1;
    connectLine['-']['O']=connectLine['O']['-']=connectLine['+']['O']=connectLine['O']['+']=1;
    connectLine['O']['O']=1;
    connectCol['|']['|']=connectCol['|']['+']=connectCol['+']['|']=connectCol['+']['+']=1;
    connectCol['|']['O']=connectCol['O']['|']=connectCol['+']['O']=connectCol['O']['+']=1;
    connectCol['O']['O']=1;

    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    // freopen("data.txt","r",stdin);

    int n,q,m;
    cin>>n>>m;
    vector<string> a(n+1);
    for(int i=1;i<=n;i++)
        cin>>a[i];
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
            a[l][r-1]=c[0];
        }
        else
        {
            DSU dsu(n*m);
            vector<int>ans(n*m);
            int cnt=0;
            for(int i=l;i<=r;i++)
                for(int j=0;j<m;j++)
                {
                    for(int k=0;k<4;k++)
                    {
                        int x=i+dx[k],y=j+dy[k];
                        if(x>=l&&x<=r&&y>=0&&y<m)
                        {
                            if(connectLine[a[i][j]][a[x][y]]&&dx[k]==0)
                                dsu.merge((i-1)*m+j,(x-1)*m+y);
                            else if(connectCol[a[i][j]][a[x][y]]&&dy[k]==0)
                                dsu.merge((i-1)*m+j,(x-1)*m+y);
                        }
                    }
                }
            for(int i=l;i<=r;i++)
                for(int j=0;j<m;j++)
                    if(a[i][j]=='O')
                        ans[dsu.find((i-1)*m+j)]|=1;
            for(int i=l;i<=r;i++)
                for(int j=0;j<m;j++)
                    if((i-1)*m+j==dsu.find((i-1)*m+j))
                        cnt+=ans[(i-1)*m+j];
            cout<<cnt<<"\n";
        }
    }

     return 0;

}