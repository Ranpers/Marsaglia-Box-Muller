#include "box_muller.hpp"
#include <iostream>
#include <iomanip>
#include <vector>
#include <thread>
#include <mutex>
#include <cmath>
#include <numeric>
#include <random>
#include <algorithm>
#include <fstream>
#include <string>

// 用于验证分布的统计数据
struct Stats {
    double mean = 0.0;
    double variance = 0.0;
    double skewness = 0.0;
    double kurtosis = 0.0;
};

// 线程安全的输出锁
std::mutex outputMutex;

// 计算统计数据
Stats calculateStats(const std::vector<double>& samples) {
    Stats stats;
    const size_t n = samples.size();
    
    if (n < 4) return stats;
    
    // 计算均值
    stats.mean = std::accumulate(samples.begin(), samples.end(), 0.0) / n;
    
    // 计算方差、偏度和峰度
    double m2 = 0.0, m3 = 0.0, m4 = 0.0;
    
    for (double x : samples) {
        const double delta = x - stats.mean;
        const double delta2 = delta * delta;
        const double delta3 = delta2 * delta;
        const double delta4 = delta3 * delta;
        
        m2 += delta2;
        m3 += delta3;
        m4 += delta4;
    }
    
    m2 /= n;
    m3 /= n;
    m4 /= n;
    
    stats.variance = m2;
    
    // 偏度和峰度的无偏估计
    if (m2 > 0) {
        stats.skewness = m3 / std::pow(m2, 1.5);
        stats.kurtosis = m4 / (m2 * m2) - 3.0; // excess kurtosis
    }
    
    return stats;
}

// 测试函数 - 每个线程独立运行
void testThread(BoxMuller& bm, int threadId, int samplesPerThread, std::vector<double>& results) {
    std::vector<double> localResults(samplesPerThread);
    
    // 生成标准正态分布随机数
    for (int i = 0; i < samplesPerThread; ++i) {
        localResults[i] = bm();
    }
    
    // 计算线程内的统计数据
    Stats localStats = calculateStats(localResults);
    
    // 输出线程统计
    {
        std::lock_guard<std::mutex> lock(outputMutex);
        std::cout << "Thread " << threadId << " stats:" << std::endl;
        std::cout << std::fixed << std::setprecision(6);
        std::cout << "  Mean:       " << localStats.mean << " (expected 0.0)" << std::endl;
        std::cout << "  Variance:   " << localStats.variance << " (expected 1.0)" << std::endl;
        std::cout << "  Skewness:   " << localStats.skewness << " (expected 0.0)" << std::endl;
        std::cout << "  Kurtosis:   " << localStats.kurtosis << " (expected 0.0)" << std::endl;
        std::cout << std::endl;
    }
    
    // 将结果合并到全局结果集
    {
        std::lock_guard<std::mutex> lock(outputMutex);
        results.insert(results.end(), localResults.begin(), localResults.end());
    }
}

// 测试指定均值和标准差的分布
void testCustomDistribution(BoxMuller& bm, double mean, double stddev, int samples) {
    std::vector<double> samplesVec(samples);
    
    for (int i = 0; i < samples; ++i) {
        samplesVec[i] = bm(mean, stddev);
    }
    
    Stats stats = calculateStats(samplesVec);
    
    std::cout << "Testing distribution with mean=" << mean 
              << ", stddev=" << stddev << std::endl;
    std::cout << std::fixed << std::setprecision(6);
    std::cout << "  Actual mean:    " << stats.mean << std::endl;
    std::cout << "  Actual variance:" << stats.variance 
              << " (expected " << stddev*stddev << ")" << std::endl;
    std::cout << "  Skewness:       " << stats.skewness << std::endl;
    std::cout << "  Kurtosis:       " << stats.kurtosis << std::endl;
    std::cout << std::endl;
}

int main() {
    try {
        // 创建BoxMuller实例
        BoxMuller bm;
        
        // 测试参数
        const int numThreads = std::thread::hardware_concurrency();
        const int samplesPerThread = 1000000;
        const int totalSamples = numThreads * samplesPerThread;
        
        std::cout << "Testing BoxMuller with " << numThreads 
                  << " threads generating " << samplesPerThread 
                  << " samples each (" << totalSamples << " total samples)." << std::endl;
        std::cout << std::endl;
        
        // 存储所有线程的结果
        std::vector<double> allResults;
        allResults.reserve(totalSamples);
        
        // 创建并运行线程
        std::vector<std::thread> threads;
        threads.reserve(numThreads);
        
        auto start = std::chrono::high_resolution_clock::now();
        
        for (int i = 0; i < numThreads; ++i) {
            threads.emplace_back(testThread, std::ref(bm), i, samplesPerThread, std::ref(allResults));
        }
        
        // 等待所有线程完成
        for (auto& t : threads) {
            t.join();
        }
        
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        
        std::cout << "All threads completed in " << duration << " ms." << std::endl;
        std::cout << std::endl;
        
        // 计算全局统计数据
        Stats globalStats = calculateStats(allResults);
        
        std::cout << "Global statistics for " << allResults.size() << " samples:" << std::endl;
        std::cout << std::fixed << std::setprecision(8);
        std::cout << "  Mean:       " << globalStats.mean << " (expected 0.0)" << std::endl;
        std::cout << "  Variance:   " << globalStats.variance << " (expected 1.0)" << std::endl;
        std::cout << "  Skewness:   " << globalStats.skewness << " (expected 0.0)" << std::endl;
        std::cout << "  Kurtosis:   " << globalStats.kurtosis << " (expected 0.0)" << std::endl;
        std::cout << std::endl;
        
        // 测试自定义分布
        testCustomDistribution(bm, 5.0, 2.0, 1000000);
        testCustomDistribution(bm, -3.0, 0.5, 1000000);
        
        // 输出直方图数据到文件（用于可视化验证）
        std::ofstream histFile("histogram_data.txt");
        if (histFile.is_open()) {
            for (double val : allResults) {
                histFile << val << "\n";
            }
            histFile.close();
            std::cout << "Histogram data written to histogram_data.txt" << std::endl;
        }
        
        return 0;
    }
    catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return 1;
    }
}    