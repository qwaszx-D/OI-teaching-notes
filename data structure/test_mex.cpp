#include <iostream>
#include <vector>
using namespace std;
int max_all = 0, max_non = 0;
int N;
void dfs(int idx, vector<int>& A, int max_val) {
    if (idx == N) {
        int c_all = 0, c_non = 0;
        for (int l = 0; l < N; ++l) {
            int mask = 0;
            for (int r = l; r < N; ++r) {
                mask |= (1 << A[r]);
                int m = 0;
                while (mask & (1 << m)) m++;
                if (l == r) {
                    c_all++;
                } else {
                    if (A[l] < m && A[r] < m) {
                        bool uniq_l = true, uniq_r = true;
                        for (int i = l + 1; i <= r; ++i) if (A[i] == A[l]) { uniq_l = false; break; }
                        if (uniq_l) {
                            for (int i = l; i < r; ++i) if (A[i] == A[r]) { uniq_r = false; break; }
                            if (uniq_r) {
                                c_all++; c_non++;
                            }
                        }
                    }
                }
            }
        }
        if (c_all > max_all) max_all = c_all;
        if (c_non > max_non) max_non = c_non;
        return;
    }
    int limit = min(max_val + 2, N);
    for (int v = 0; v < limit; ++v) {
        A.push_back(v);
        dfs(idx + 1, A, max(max_val, v));
        A.pop_back();
    }
}
int main(int argc, char** argv) {
    N = atoi(argv[1]);
    vector<int> A;
    dfs(0, A, -1);
    cout << max_all << " " << max_non << endl;
    return 0;
}