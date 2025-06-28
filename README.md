## <u>概述</u>
本项目基于 Marsaglia 极坐标法实现了 Box-Muller 算法，可高效生成高斯分布随机数。该实现具有以下特点：
1. 线程安全 - 采用线程本地存储(TLS)技术，确保多线程环境下的可靠运行
2. 跨构建支持 - 提供动态库和静态库两种构建方式
3. **平台限定 - 由于使用 Linux 特有的 `getrandom()` 系统调用，不兼容其他操作系统**

## <u>核心特性</u>
- **线程安全设计**：采用线程本地存储(TLS)技术实现无锁并行，确保多线程环境下的高性能
- **高质量随机源**：结合系统级 `getrandom` 接口获取高质量熵源
- **灵活接口**：支持生成标准正态分布和自定义参数的正态分布
- **完备测试**：提供多线程测试验证统计特性

## <u>项目结构</u>
```plaintext
Marsaglia-Box-Muller/
├── CMakeLists.txt          # 项目构建配置
├── boxmuller_auto_test.sh  # 一键式编译测试脚本
├── include/
│   └── box_muller.hpp      # 公共接口定义
├── src/
│   └── box_muller.cpp      # 核心算法实现
└── test/
    └── test.cpp            # 统计特性验证程序
```

## <u>快速开始</u>

### 自动构建并测试（推荐）
```bash
chmod +x boxmuller_auto_test.sh
./boxmuller_auto_test.sh
```

### 手动构建
```bash
mkdir build && cd build
cmake .. && make
```

### 手动测试
参考 boxmuller_auto_test.sh 中的测试步骤


### 库文件集成
项目生成以下库文件：
- 动态库: `libbox_muller_shared.so`
- 静态库: `libbox_muller_static.a`

推荐集成方式：
```bash
g++ -std=c++17 -pthread your_app.cpp -Iinclude -L./lib -lbox_muller_shared \
    -Wl,-rpath='$ORIGIN/lib' -o your_app
```

## <u>质量验证</u>
测试程序自动验证以下统计特性：
1. 均值收敛性
2. 方差稳定性
3. 偏度和峰度检验
4. 多线程一致性

测试结果输出包括：
- 控制台统计报告
- `histogram_data.txt` 数据文件（可用于可视化）
### 测试结果（仅供参考）
```
# 环境：WSL2 Debian  Intel Ultra7 155H
Testing BoxMuller with 22 threads generating 1000000 samples each (22000000 total samples).

Thread 14 stats:
  Mean:       0.001098 (expected 0.0)
  Variance:   1.000683 (expected 1.0)
  Skewness:   0.001272 (expected 0.0)
  Kurtosis:   -0.002322 (expected 0.0)

Thread 17 stats:
  Mean:       -0.000205 (expected 0.0)
  Variance:   0.999223 (expected 1.0)
  Skewness:   0.003196 (expected 0.0)
  Kurtosis:   -0.003269 (expected 0.0)

Thread 2 stats:
  Mean:       -0.001324 (expected 0.0)
  Variance:   1.001900 (expected 1.0)
  Skewness:   -0.004083 (expected 0.0)
  Kurtosis:   -0.001907 (expected 0.0)

Thread 21 stats:
  Mean:       -0.000293 (expected 0.0)
  Variance:   1.000281 (expected 1.0)
  Skewness:   0.001036 (expected 0.0)
  Kurtosis:   -0.000335 (expected 0.0)

Thread 7 stats:
  Mean:       0.001558 (expected 0.0)
  Variance:   0.999792 (expected 1.0)
  Skewness:   -0.003654 (expected 0.0)
  Kurtosis:   -0.001818 (expected 0.0)

Thread 3 stats:
  Mean:       0.000544 (expected 0.0)
  Variance:   1.000514 (expected 1.0)
  Skewness:   0.001033 (expected 0.0)
  Kurtosis:   0.003939 (expected 0.0)

Thread 20 stats:
  Mean:       0.000421 (expected 0.0)
  Variance:   0.998899 (expected 1.0)
  Skewness:   0.003863 (expected 0.0)
  Kurtosis:   -0.002861 (expected 0.0)

Thread 16 stats:
  Mean:       -0.000231 (expected 0.0)
  Variance:   1.000065 (expected 1.0)
  Skewness:   -0.002223 (expected 0.0)
  Kurtosis:   -0.007369 (expected 0.0)

Thread 6 stats:
  Mean:       -0.000478 (expected 0.0)
  Variance:   0.998455 (expected 1.0)
  Skewness:   -0.001004 (expected 0.0)
  Kurtosis:   -0.001987 (expected 0.0)

Thread 19 stats:
  Mean:       0.001246 (expected 0.0)
  Variance:   0.998557 (expected 1.0)
  Skewness:   -0.000564 (expected 0.0)
  Kurtosis:   -0.006595 (expected 0.0)

Thread 1 stats:
  Mean:       0.000164 (expected 0.0)
  Variance:   0.998821 (expected 1.0)
  Skewness:   0.004648 (expected 0.0)
  Kurtosis:   -0.001631 (expected 0.0)

Thread 12 stats:
  Mean:       -0.000104 (expected 0.0)
  Variance:   0.998972 (expected 1.0)
  Skewness:   0.001865 (expected 0.0)
  Kurtosis:   -0.009909 (expected 0.0)

Thread 10 stats:
  Mean:       -0.001447 (expected 0.0)
  Variance:   0.999535 (expected 1.0)
  Skewness:   0.003365 (expected 0.0)
  Kurtosis:   -0.003906 (expected 0.0)

Thread 9 stats:
  Mean:       -0.000826 (expected 0.0)
  Variance:   0.999122 (expected 1.0)
  Skewness:   -0.002969 (expected 0.0)
  Kurtosis:   -0.006213 (expected 0.0)

Thread 5 stats:
  Mean:       0.001118 (expected 0.0)
  Variance:   0.998231 (expected 1.0)
  Skewness:   0.000448 (expected 0.0)
  Kurtosis:   0.000695 (expected 0.0)

Thread 8 stats:
  Mean:       -0.000149 (expected 0.0)
  Variance:   0.999599 (expected 1.0)
  Skewness:   0.005498 (expected 0.0)
  Kurtosis:   -0.003856 (expected 0.0)

Thread 15 stats:
  Mean:       -0.002001 (expected 0.0)
  Variance:   0.997759 (expected 1.0)
  Skewness:   -0.002253 (expected 0.0)
  Kurtosis:   -0.006466 (expected 0.0)

Thread 13 stats:
  Mean:       -0.001036 (expected 0.0)
  Variance:   0.999075 (expected 1.0)
  Skewness:   0.000878 (expected 0.0)
  Kurtosis:   0.004879 (expected 0.0)

Thread 0 stats:
  Mean:       0.000372 (expected 0.0)
  Variance:   0.998975 (expected 1.0)
  Skewness:   0.000197 (expected 0.0)
  Kurtosis:   0.001002 (expected 0.0)

Thread 18 stats:
  Mean:       0.000603 (expected 0.0)
  Variance:   1.002036 (expected 1.0)
  Skewness:   0.000675 (expected 0.0)
  Kurtosis:   -0.005755 (expected 0.0)

Thread 11 stats:
  Mean:       0.002606 (expected 0.0)
  Variance:   0.999925 (expected 1.0)
  Skewness:   -0.000680 (expected 0.0)
  Kurtosis:   0.000687 (expected 0.0)

Thread 4 stats:
  Mean:       -0.000076 (expected 0.0)
  Variance:   0.998816 (expected 1.0)
  Skewness:   0.000990 (expected 0.0)
  Kurtosis:   0.001063 (expected 0.0)

All threads completed in 650 ms.

Global statistics for 22000000 samples:
  Mean:       0.00007090 (expected 0.0)
  Variance:   0.99951183 (expected 1.0)
  Skewness:   0.00052400 (expected 0.0)
  Kurtosis:   -0.00244744 (expected 0.0)

Testing distribution with mean=5.00000000, stddev=2.00000000
  Actual mean:    4.998921
  Actual variance:4.004866 (expected 4.000000)
  Skewness:       0.002207
  Kurtosis:       -0.008074

Testing distribution with mean=-3.000000, stddev=0.500000
  Actual mean:    -2.999473
  Actual variance:0.249961 (expected 0.250000)
  Skewness:       0.001177
  Kurtosis:       -0.003320
```

## <u>使用指南</u>

### 基础用法
```cpp
#include "box_muller.hpp"

BoxMuller rng;  // 创建生成器实例

// 生成标准正态分布随机数
double std_rand = rng();

// 生成自定义参数的正态分布随机数
double custom_rand = rng(5.0, 2.0);  // μ=5.0, σ=2.0
```

## <u>部署选项</u>
```bash
make install  # 安装到系统目录
```

## <u>贡献指南</u>
欢迎通过 issue 或 pull request 提交改进建议。请确保：
- 代码风格保持一致
- 新增功能附带测试用例
- 通过现有测试套件

## <u>联系信息</u>
- 作者：Yiran
- 邮箱：yi_ran@aliyun.com
