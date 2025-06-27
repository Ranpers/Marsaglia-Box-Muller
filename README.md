# Marsaglia-Box-Muller 随机数生成器

## 概述
本项目基于 Marsaglia 极坐标法实现了 Box-Muller 算法，可高效生成正态分布随机数。该实现具有以下特点：
1. 线程安全 - 采用线程本地存储(TLS)技术，确保多线程环境下的可靠运行
2. 跨构建支持 - 提供动态库和静态库两种构建方式
3. **平台限定 - 由于使用 Linux 特有的 `getrandom()` 系统调用，不兼容其他操作系统**

## 核心特性
- **线程安全设计**：采用线程本地存储(TLS)技术实现无锁并行，确保多线程环境下的高性能
- **高质量随机源**：结合系统级 `getrandom` 接口获取高质量熵源
- **灵活接口**：支持生成标准正态分布和自定义参数的正态分布
- **完备测试**：提供多线程测试验证统计特性

## 项目结构
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

## 快速开始

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

## 使用指南

### 基础用法
```cpp
#include "box_muller.hpp"

BoxMuller rng;  // 创建生成器实例

// 生成标准正态分布随机数
double std_rand = rng();

// 生成自定义参数的正态分布随机数
double custom_rand = rng(5.0, 2.0);  // μ=5.0, σ=2.0
```

### 库文件集成
项目生成以下库文件：
- 动态库: `libbox_muller_shared.so`
- 静态库: `libbox_muller_static.a`

推荐集成方式：
```bash
g++ -std=c++17 -pthread your_app.cpp -Iinclude -L./lib -lbox_muller_shared \
    -Wl,-rpath='$ORIGIN/lib' -o your_app
```

## 质量验证
测试程序自动验证以下统计特性：
1. 均值收敛性
2. 方差稳定性
3. 偏度和峰度检验
4. 多线程一致性

测试结果输出包括：
- 控制台统计报告
- `histogram_data.txt` 数据文件（可用于可视化）

## 部署选项
```bash
make install  # 安装到系统目录
```

## 贡献指南
欢迎通过 issue 或 pull request 提交改进建议。请确保：
- 代码风格保持一致
- 新增功能附带测试用例
- 通过现有测试套件

## 联系信息
- 作者：Yiran
- 邮箱：yi_ran@aliyun.com

## 许可声明
详见项目中的许可证文件（如有）
