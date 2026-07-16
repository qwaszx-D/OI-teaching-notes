#include<bits/stdc++.h>
using namespace std;
inline int read(){
	int x = 0, f = 1; char ch = getchar();
	while(!isdigit(ch)) (ch == '-') && (f = -1), ch = getchar();
	while(isdigit(ch)) x = (x << 1) + (x << 3) + (ch ^ 48), ch = getchar();
	return x * f;
}
#define N 2005
#define ll long long
int n, A, B, g[N];
ll d[N], sum;
bool f[N][N];
ll solve1(){
	ll ans = 0, res = 0;
	int maxn = log2(sum) + 1;//sum为每一个Y的和，这是确定枚举的最高位
	for(int i = maxn; i >= 0; i--){
		res = ans | ((1LL << i) - 1LL);
		memset(f, false, sizeof(f));//每次枚举都要清空
		f[0][0] = true;//初始值
        for(int j = 1; j <= n; j++){//枚举第几个数
            for(int k = 1; k <= min(j, B); k++){//枚举分段数
                ll s = 0;
                for(int t = j - 1; t >= k - 1; t--){//枚举分段位置
                    s += d[t + 1];//s表示t+1~j的Y的和
                    if(f[t][k - 1] && ((s | res) == res)){
                        f[j][k] = true;
                        break;
                    }
                }
            }
        }
        bool flag = false;
		for(int j = A; j <= B; j++){//枚举分A到B段的所有情况
			if(f[n][j]){
				flag = true;
				break;
			}
		}
		if(!flag)ans |= (1LL << i);//不可以放0则只能放1
	}
    return ans;
}
ll solve2(){
	ll ans = 0, res = 0;
	int maxn = log2(sum) + 1;
	for(int i = maxn; i >= 0; i--){
		res = ans | ((1LL << i) - 1LL);
		memset(g, 127, sizeof(g));
		g[0] = 0;
		for(int j = 1; j <= n; j++){
			ll s = 0;
			for(int k = j - 1; k >= 0; k--){
				s += d[k + 1];
				if((s | res) == res)
					g[j] = min(g[j], g[k] + 1);
			}
		}
		if(g[n] > B) ans |= (1LL << i);//如果得到0的最小分段数大于B，那么这一位只能为1
	}
	return ans;
}
int main(){
	//freopen("sculpture.in", "r", stdin);
	//freopen("sculpture.out", "w", stdout);
	n = read(), A = read(), B = read();
	for(int i = 1; i <= n; i++) d[i] = read(), sum += d[i];
	if(A == 1)	printf("%lld", solve2());
	else printf("%lld", solve1());
	return 0;
}

