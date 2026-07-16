#include <bits/stdc++.h>
using namespace std;

int main() {
    int i = 0;
    while (true) {
        cout << "case " << ++i << endl;
        system("gen.exe > in.txt");
        system("brute.exe < in.txt > ans.txt");
        system("std.exe < in.txt > out.txt");
        if (system("fc ans.txt out.txt")) break;
    }
    return 0;
}