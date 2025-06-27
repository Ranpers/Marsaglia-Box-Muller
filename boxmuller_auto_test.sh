#!/bin/bash

# 配置部分
SRC_DIR="src"             # 源代码目录
BUILD_DIR="build"         # 构建目录
TEST_DIR="test"           # 测试目录
LIB_DIR="lib"
INCLUDE_DIR="include"
SHARE="libbox_muller_shared.so" 
STATIC="libbox_muller_static.a"
TEST_EXECUTABLE="test"
TEST_DATA="histogram_data.txt"

# 颜色定义，用于美化输出
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
NC='\033[0m' # 无颜色

# 函数：打印错误信息并退出
function error_exit {
    echo -e "${RED}[错误]${NC} $1" 1>&2
    exit 1
}

# 函数：打印成功信息
function success_msg {
    echo -e "${GREEN}[成功]${NC} $1"
}

# 函数：打印提示信息
function info_msg {
    echo -e "${YELLOW}[提示]${NC} $1"
}

# 检查必要的工具是否存在
command -v g++ >/dev/null 2>&1 || error_exit "g++ 未找到，请安装 g++"
command -v make >/dev/null 2>&1 || error_exit "make 未找到，请安装 make"

# 删除已存在的build目录（如果存在）
if [ -d "$BUILD_DIR" ]; then
    info_msg "删除已存在的 $BUILD_DIR 目录..."
    rm -rf "$BUILD_DIR" || error_exit "无法删除 $BUILD_DIR 目录"
fi

if [ -f "$TEST_DIR/$TEST_DATA" ]; then
    info_msg "删除已存在的测试结果统计文件..."
    rm "$TEST_DIR/$TEST_DATA" || error_exit "无法删除测试结果统计文件"
fi

if [ -d "$TEST_DIR/$INCLUDE_DIR" ]; then
    info_msg "清空 $TEST_DIR/$INCLUDE_DIR 目录内容..."
    find "$TEST_DIR/$INCLUDE_DIR" -mindepth 1 -delete || error_exit "无法清空 $TEST_DIR/$INCLUDE_DIR 目录"
fi

if [ -d "$TEST_DIR/$LIB_DIR" ]; then
    info_msg "清空 $TEST_DIR/$LIB_DIR 目录内容..."
    find "$TEST_DIR/$LIB_DIR" -mindepth 1 -delete || error_exit "无法清空 $TEST_DIR/$LIB_DIR 目录"
fi

# 创建必要的目录
mkdir -p $BUILD_DIR

# 步骤 1: 编译源代码
info_msg "开始编译源代码..."
cd $BUILD_DIR || error_exit "无法进入 $BUILD_DIR 目录"

# 可以根据项目需求修改编译命令
if ! cmake ..; then
    error_exit "cmake失败"
fi

if ! make; then
    error_exit "make失败"
fi

cd .. || error_exit "无法进入项目根目录"

success_msg "编译完成"

# 步骤 2: 移动文件
info_msg "开始移动文件..."
cp $INCLUDE_DIR/* $TEST_DIR/$INCLUDE_DIR/ || error_exit "复制头文件失败"
mv $BUILD_DIR/$SHARE $TEST_DIR/$LIB_DIR/ || error_exit "移动依赖文件失败"
success_msg "文件移动完成"

cd $TEST_DIR || error_exit "无法进入 $TEST_DIR 目录"

# 步骤 3: 编译测试文件
if ! g++ -std=c++17 -pthread test.cpp -Iinclude -L./lib -lbox_muller_shared -Wl,-rpath='$ORIGIN/lib' -o test; then
    error_exit "测试编译失败"
fi

info_msg "开始执行测试..."

if ! ./$TEST_EXECUTABLE; then
    error_exit "测试执行失败"
fi

success_msg "测试执行完成!" 