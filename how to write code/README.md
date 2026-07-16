# C++ 对拍完整示例

这套文件用于课堂展示“如何写对拍”：

1. `fast.cpp`：待检查的正解，线段树，复杂度 `O(m log n)`。
2. `brute.cpp`：暴力程序，朴素模拟，复杂度 `O(nm)`。
3. `gen.cpp`：随机数据生成器，只生成小数据，方便暴力跑得动。
4. `stress_linux.sh`：Linux/macOS 下的对拍脚本。
5. `stress_win.bat`：Windows CMD/PowerShell 下的对拍脚本。
6. `fast_bug.cpp`：故意写错的版本，方便课堂演示“对拍抓 bug”。
7. `problem.md`：示例题面。

## Linux / macOS 用法

```bash
chmod +x stress_linux.sh
./stress_linux.sh
```

默认跑 1000 组数据。也可以指定组数：

```bash
./stress_linux.sh 5000
```

演示错误版本：

```bash
./stress_linux.sh 100 fast_bug.cpp
```

如果想开 sanitizer 调试，可以这样跑：

```bash
CXXFLAGS="-std=c++17 -O0 -g -Wall -Wextra -fsanitize=address,undefined" ./stress_linux.sh 1000
```

## Windows 用法

需要先安装能在命令行里使用的 `g++`，例如 MinGW-w64 / MSYS2 / TDM-GCC，并确保 `g++` 已经加入 `PATH`。

在 CMD 或 PowerShell 里进入本目录，然后运行：

```bat
stress_win.bat
```

默认跑 1000 组数据。也可以指定组数：

```bat
stress_win.bat 5000
```

演示错误版本：

```bat
stress_win.bat 100 fast_bug.cpp
```

## 对拍脚本做了什么

每一轮对拍流程是：

```text
gen 生成 input.txt
fast 读 input.txt，输出 out_fast.txt
brute 读 input.txt，输出 out_brute.txt
比较 out_fast.txt 和 out_brute.txt
如果不同，就打印 input.txt 和两份输出，然后停止
```

## 课堂展示建议

先运行正确版本，看到 `Accepted`。然后运行：

```bash
./stress_linux.sh 100 fast_bug.cpp
```

或者 Windows 下：

```bat
stress_win.bat 100 fast_bug.cpp
```

学生会看到某一组随机数据让两个程序输出不同。此时可以强调：

- 对拍不是证明程序正确，只是高效找 bug。
- 暴力程序一定要尽量简单，越简单越可信。
- 生成器要多覆盖边界情况，比如 `n=1`、`l=r`、负数、连续修改、连续查询等。
- 一旦发现错误，脚本保留 `input.txt`，可以直接拿这组数据调试。
