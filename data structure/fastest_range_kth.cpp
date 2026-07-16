// 洛谷 P3834：区间第 k 小
// 整体二分 (数据通道与询问通道物理隔离版 1log，0分支出膛)

#include <bits/stdc++.h>
using namespace std;

const int MAXN = 200005;
const int MAXM = 200005;

// === 核心奥义：数据与查询彻底分离 ===
struct Data {
    int pos, val;
};
struct Query {
    int pos, id, sign; // sign: -1 为左端点，1 为右端点
};

Data d_arr[MAXN], temp_d[MAXN];
Query q_arr[MAXM * 2], temp_q[MAXM * 2];
int a[MAXN], disc[MAXN];
int ans[MAXM], k_arr[MAXM], cur_cnt[MAXM];

void solve(int vl, int vr, int dL, int dR, int qL, int qR) {
    // 致命剪枝：如果在某一个值域区间里没有查询了，直接斩断所有相关数据排序操作！
    if (qL > qR) return; 

    if (vl == vr) {
        for (int i = qL; i <= qR; ++i) {
            // 到达叶子节点，双端点都会来到这里，挑一个端点记录答案即可
            if (q_arr[i].sign == 1) {
                ans[q_arr[i].id] = disc[vl - 1]; 
            }
        }
        return;
    }

    int vm = (vl + vr) >> 1;
    int add_cnt = 0;
    int d_ptr = dL;
    
    // 第一遍扫描：用非分支指令同步更新，完美双指针
    for (int i = qL; i <= qR; ++i) {
        while (d_ptr <= dR && d_arr[d_ptr].pos <= q_arr[i].pos) {
            add_cnt += (d_arr[d_ptr].val <= vm); // 彻底消除 if (type == 0) 的分支预测惩罚
            d_ptr++;
        }
        cur_cnt[q_arr[i].id] += q_arr[i].sign * add_cnt;
    }

    // 第二遍扫描：为数据元素进行空间分配和赋值
    int left_d_size = 0;
    for (int i = dL; i <= dR; ++i) {
        left_d_size += (d_arr[i].val <= vm);
    }
    int p1_d = dL, p2_d = dL + left_d_size;
    for (int i = dL; i <= dR; ++i) {
        if (d_arr[i].val <= vm) temp_d[p1_d++] = d_arr[i];
        else temp_d[p2_d++] = d_arr[i];
    }

    // 第三遍扫描：为查询端点进行空间分配和赋值
    int left_q_size = 0;
    for (int i = qL; i <= qR; ++i) {
        left_q_size += (cur_cnt[q_arr[i].id] >= k_arr[q_arr[i].id]);
    }
    int p1_q = qL, p2_q = qL + left_q_size;
    for (int i = qL; i <= qR; ++i) {
        int id = q_arr[i].id;
        if (cur_cnt[id] >= k_arr[id]) {
            temp_q[p1_q++] = q_arr[i];
        } else {
            temp_q[p2_q++] = q_arr[i];
        }
    }

    // 第四遍扫描：清空游离脏数据并平移回原数组
    for (int i = qL; i <= qR; ++i) {
        int id = q_arr[i].id;
        // 一旦 > 0 意味着我们正在结算这对查询双生子的第一次。因为紧接着就会被清零。
        if (cur_cnt[id] > 0) {
            if (cur_cnt[id] < k_arr[id]) k_arr[id] -= cur_cnt[id];
            cur_cnt[id] = 0; 
        }
        q_arr[i] = temp_q[i];
    }
    for (int i = dL; i <= dR; ++i) {
        d_arr[i] = temp_d[i];
    }

    solve(vl, vm, dL, dL + left_d_size - 1, qL, qL + left_q_size - 1);
    solve(vm + 1, vr, dL + left_d_size, dR, qL + left_q_size, qR);
}

inline int read() {
    int x = 0, f = 1;
    char ch = getchar();
    while (ch < '0' || ch > '9') {
        if (ch == '-') f = -1;
        ch = getchar();
    }
    while (ch >= '0' && ch <= '9') {
        x = x * 10 + ch - '0';
        ch = getchar();
    }
    return x * f;
}

int main() {
    int n = read();
    int m = read();

    int tot_disc = 0;
    int tot_d = 0, tot_q = 0;
    
    for (int i = 1; i <= n; ++i) {
        a[i] = read();
        disc[tot_disc++] = a[i];
    }

    for (int i = 0; i < m; ++i) {
        int l = read();
        int r = read();
        k_arr[i] = read();
        ans[i] = -1;
        q_arr[tot_q++] = {l - 1, i, -1};
        q_arr[tot_q++] = {r, i, 1};
    }

    sort(disc, disc + tot_disc);
    tot_disc = unique(disc, disc + tot_disc) - disc;

    for (int i = 1; i <= n; ++i) {
        int v = lower_bound(disc, disc + tot_disc, a[i]) - disc + 1;
        d_arr[tot_d++] = {i, v};
    }

    // d_arr 自身已经是天然按 pos 有序，无需再一次排序宏运算！
    // 仅仅只需要单独让游离出去的端点按 pos 组排序
    sort(q_arr, q_arr + tot_q, [](const Query& A, const Query& B) {
        return A.pos < B.pos;
    });

    solve(1, tot_disc, 0, tot_d - 1, 0, tot_q - 1);

    for (int i = 0; i < m; ++i) {
        printf("%d\n", ans[i]);
    }
    return 0;
}