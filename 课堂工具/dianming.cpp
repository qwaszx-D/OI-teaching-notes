#include<iostream>
#include<cstdio>
#include<cstring>
#include<windows.h>
#include<cassert>
#include<functional>
#include<random>
#include<ctime>
#include<set>
#include<cmath>
#include<algorithm>
using namespace std;
const int mod=998244353;
mt19937 rnd(GetTickCount());
int random(int l,int r){return rnd()%(r-l+1)+l;}
template<int mod>struct F
{
	int x;
	F(int v=0){x=qmod2(v%mod);}
	F(unsigned long long v){x = v % mod;}
	int qmod1(const int &x)const{return x>=mod?x-mod:x;}
	int qmod2(const int &x)const{return x+(x>>31&mod);}
	int qpower(int a,int b)const
	{
		int ans=1;for(;b;b>>=1,a=1ll*a*a%mod)if(b&1)ans=1ll*ans*a%mod;
		return ans;
	}
	F& operator +=(const F &b){x+=b.x;if(x>=mod)x-=mod;return *this;}
	F& operator -=(const F &b){x-=b.x;x+=x>>31&mod;return *this;}
	F& operator *=(const F &b){x=1ll*x*b.x%mod;return *this;}
	F inv()const{return qpower(x,mod-2);}
	F& operator /=(const F &b){x=1ll*x*qpower(b.x,mod-2)%mod;return *this;}
	F operator -()const{return (F){qmod2(-x)};}
    int toInt()const{return x;}
};
//vector<F<mod>>a;
int a[1000005];
int b[1000005];
using T=long long;
T c[1000005];
long long f[1000005],g[1000005];
inline float FrDielectric(float cosThetaI, float etaI, float etaT) {
  bool entering = cosThetaI > 0.0f;
  if (!entering) {
    std::swap(etaI, etaT);
    cosThetaI = std::fabs(cosThetaI);
  }

  float sinThetaI = std::sqrt(std::max(0.0f, 1 - cosThetaI * cosThetaI));
  float sinThetaT = etaI / etaT * sinThetaI;
  if (sinThetaT >= 1)
    return 1;
  float cosThetaT = std::sqrt(std::max(0.0f, 1 - sinThetaT * sinThetaT));
  float Rparl = ((etaT * cosThetaI) - (etaI * cosThetaT)) /
                ((etaT * cosThetaI) + (etaI * cosThetaT));

  float Rperp = ((etaI * cosThetaI) - etaT * (cosThetaT)) /
                ((etaI * cosThetaI) + (etaT * cosThetaT));
  return (Rparl * Rparl + Rperp * Rperp) / 2;
}
#include<queue>
set<int>test[1000000];
void dfs(int n)
{
	if(n==1)
	{
		cout<<'?';
		return;
	}
	cout<<'(';
	int t=random(1,n-1);
	dfs(t);
	switch(random(0,2))
	{
		case 0:cout<<'&';break;
		case 1:cout<<'|';break;
		case 2:cout<<'^';break;
	}
	dfs(n-t);
	cout<<')';
}
int id[1000000];
int main()
{
	// freopen("data.txt","w",stdout);
	srand(time(0));
	ios::sync_with_stdio(0);
	cin.tie(0);
	int n=16, m=random(1,20);
    cout<<random(1,n)<<endl;
    return 0;
	cout<<n<<" "<<m<<endl;
	for(int i=1;i<=m;i++)
	{
		int u=random(1,n), v=random(1,n);
		cout<<u<<" "<<v<<"\n";
	}
	return 0;
	cout<<random(1,13)<<endl;
	return 0;
	int T=10;
	cout<<T<<"\n";
	for(int i=1;i<=T;i++)
	{
		int n=10, m=6;
		cout<<n<<" "<<m<<"\n";
		for(int i=1;i<=n;i++)
			cout<<random(-20,0)<<" ";
		cout<<"\n";
		for(int i=1;i<=n;i++)
			cout<<random(0,20)<<" ";
		cout<<"\n";
		for(int i=1;i<=m;i++)
		{
			int x=random(1,n), y=random(1,n), z=random(-5,5);
			cout<<x<<" "<<y<<" "<<z<<"\n";
		}
	}
	return 0;
	

	// int n=random(10,10);
	// cout<<n<<"\n";
	// for(int i=2;i<=n;i++)
	// 	cout<<random(i-1,i-1)<<"\n";
	// for(int i=1;i<=n;i++)
	// 	cout<<(random(0,1)?'(':')')<<'\n';
	// return 0;

}
