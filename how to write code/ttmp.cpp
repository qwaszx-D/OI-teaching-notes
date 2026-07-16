#include <bits/stdc++.h>
using namespace std;

int main() {
    vector<int> a;        // 空 vector
    vector<int> b(5);     // 5 个 int，初始为 0
    vector<int> c(5, 7);  // 5 个 int，每个都是 7
    for(int i=0;i<5;i++) {
        cout << c[i] << " "; 
    }
}

编译器 翻译成 二进制（机器语言）

运行 二进制程序