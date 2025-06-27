#include "box_muller.hpp"

#include <cmath>
#include <limits>
#include <stdexcept>
#include <sys/random.h>

// 匿名命名空间保护实现细节
namespace
{

    // 线程本地存储的状态结构
    struct ThreadLocalState
    {
        bool hasSpare = false; // 标记是否有缓存值
        double spare = 0.0;    // 缓存的值
    };

    // 定义线程本地变量
    thread_local ThreadLocalState tls_state;

} // 匿名命名空间

double BoxMuller::getUniform()
{
    uint64_t random_bits;

    while (true)
    {
        // 使用系统调用获取高质量随机数
        if (getrandom(&random_bits, sizeof(random_bits), 0) != sizeof(random_bits))
        {
            throw std::runtime_error("Failed to get random bits from system");
        }

        // 将64位无符号整数映射到(0,1)开区间
        constexpr double divisor = static_cast<double>(UINT64_MAX) + 1.0;
        const double u = (static_cast<double>(random_bits) + 0.5) / divisor;

        // 严格检查确保在(0,1)区间内
        if (u > 0.0 && u < 1.0)
        {
            return 2.0 * u - 1.0; // 映射到(-1,1)
        }
        // 极低概率情况下会循环重试
    }
}

double BoxMuller::operator()()
{
    // 检查线程本地存储中是否有缓存的随机数
    if (tls_state.hasSpare)
    {
        tls_state.hasSpare = false;
        return tls_state.spare;
    }

    double u, v, s;
    constexpr double min_s = 4.0 * std::numeric_limits<double>::min();

    // Marsaglia极坐标法
    do
    {
        u = getUniform();
        v = getUniform();
        s = u * u + v * v;
    } while (s >= 1.0 || s <= min_s);

    // Box-Muller变换
    const double root = std::sqrt(-2.0 * std::log(s) / s);
    tls_state.spare = v * root;
    tls_state.hasSpare = true;
    return u * root;
}

double BoxMuller::operator()(double mean, double stddev)
{
    // 参数校验
    if (stddev < 0.0)
    {
        throw std::invalid_argument("Standard deviation must be non-negative");
    }

    return mean + stddev * (*this)();
}