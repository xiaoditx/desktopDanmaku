#ifndef RANDNUM_H
#define RANDNUM_H

// 随机数生成命名空间
namespace random
{
    /**
     * @brief 在[min,max]区间内获取随机整数
     *
     * @param[in] min 区间左端点，也就是随机数最小值
     * @param[in] max 区间右端点，也就是最大值
     *
     * @return 一个随机整数值
     */
    int getInt(int min, int max);

    /**
     * @brief 在[min,max)区间内获取随机double型数字
     *
     * @param[in] min 区间左端点，也就是随机数最小值
     * @param[in] max 区间右端点，也就是最大值
     *
     * @return 一个随机double值
     *
     * @note 本函数取值区间左开右闭，取不到右端点
     */
    double getDouble(double min, double max);
}

#endif // RANDNUM_H
