#include "functions/randnum.hpp"
#include <random>
#include <chrono>
#include <algorithm>

namespace random
{
    // 获取全局随机数生成器的单例引用
    // 使用Mersenne Twister 19937算法，线程安全（C++11保证静态局部变量初始化线程安全）
    static std::mt19937 &getGenerator()
    {
        // 静态局部变量，首次调用时初始化，后续调用返回同一实例
        static std::mt19937 generator(
            []() // 使用lambda表达式进行延迟初始化
            {
                // 创建真随机数设备，用于获取随机种子
                std::random_device rd;

                // 混合两种随机源生成种子：
                // 1. random_device的真随机数
                // 2. 当前时间戳（纳秒计数）
                // 使用异或操作混合两个随机源，增加熵值
                auto seed = rd() ^ static_cast<unsigned int>(
                                       std::chrono::system_clock::now().time_since_epoch().count());

                // 使用混合后的种子初始化Mersenne Twister生成器
                std::mt19937 gen(seed);
                // 预热，丢弃前10个随机数，Mersenne Twister初始状态的前几个数值随机性较差
                gen.discard(10);
                return gen;
            }()); // lambda立即调用，返回初始化好的生成器
        return generator;
    }
    // 生成[min, max]范围内的均匀分布随机整数
    // 参数：
    //   min - 范围下限（包含）
    //   max - 范围上限（包含）
    // 返回值：范围内的随机整数
    int getInt(int min, int max)
    {
        // 确保min <= max，如果输入相反则交换
        if (min > max)
            std::swap(min, max);
        // 创建均匀整数分布器
        std::uniform_int_distribution<int> dist(min, max);
        // 使用全局生成器产生随机数
        return dist(getGenerator());
    }
    // 生成[min, max)范围内的均匀分布随机浮点数（左闭右开）
    // 参数：
    //   min - 范围下限（包含）
    //   max - 范围上限（不包含）
    // 返回值：范围内的随机浮点数
    double getDouble(double min, double max)
    {
        // 确保min <= max，如果输入相反则交换
        if (min > max)
            std::swap(min, max);
        // 创建均匀实数分布器（默认区间为[min, max)）
        std::uniform_real_distribution<double> dist(min, max);
        // 使用全局生成器产生随机数
        return dist(getGenerator());
    }
}
