#ifndef BOX_MULLER_HPP
#define BOX_MULLER_HPP

#include <cstdint>
#include <limits>

/**
 * @class BoxMuller
 * @brief 使用Box-Muller算法生成正态分布随机数的线程安全实现
 *
 * 本实现使用线程本地存储(TLS)实现无锁并行，适合多线程环境使用。
 * 算法基于Marsaglia极坐标法，生成高质量的随机数。
 */
class BoxMuller
{
public:
    BoxMuller() = default;
    ~BoxMuller() = default;

    // 禁用拷贝和移动语义
    BoxMuller(const BoxMuller &) = delete;
    BoxMuller &operator=(const BoxMuller &) = delete;
    BoxMuller(BoxMuller &&) = delete;
    BoxMuller &operator=(BoxMuller &&) = delete;

    /**
     * @brief 生成标准正态分布随机数(均值0，标准差1)
     * @return double 标准正态分布随机数
     */
    double operator()();

    /**
     * @brief 生成指定参数的正态分布随机数
     * @param mean 分布的均值
     * @param stddev 分布的标准差
     * @return double 正态分布随机数
     */
    double operator()(double mean, double stddev);

private:
    /**
     * @brief 生成(-1,1)区间的均匀分布随机数
     * @return double (-1,1)区间的均匀分布随机数
     * @note 使用系统getrandom接口获取高质量随机数
     */
    double getUniform();
};

#endif // BOX_MULLER_HPP