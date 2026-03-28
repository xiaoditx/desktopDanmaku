#include "pch.hpp"
#include "functions/randnum.hpp"

namespace random
{
    // 基于 PCG 算法的伪随机数生成器
    class RmPcg32
    {
    public:
        using TSeed = UINT64;

        // 乘数和增量
        constexpr static UINT64 Multiplier = 6364136223846793005ull;
        constexpr static UINT64 Increment = 1442695040888963407ull;

        // 默认种子，使用系统时间戳
        static UINT64 DefaultSeed() noexcept { return GetTickCount64(); }

    private:
        UINT64 State;

    public:
        constexpr RmPcg32(UINT64 Seed = GetTickCount64()) noexcept : State{Seed + Increment} {}

        // 生成一个随机的32位无符号整数
        constexpr UINT Next32() noexcept
        {
            const auto Old = State;
            State = Old * Multiplier + Increment;
            const auto XorShifted = UINT((Old ^ (Old >> 18)) >> 27);
            const auto Rot = UINT(Old >> 59);
            return XorShifted >> Rot | XorShifted << (-Rot & 31);
        }

        // 重新设置种子，使用新的种子值初始化状态
        constexpr void Seed(UINT64 Seed) noexcept { State = Seed + Increment; }
    };

    template <class TBase>
    struct Random : public TBase
    {
        using TBase::Next32;

        constexpr Random(typename TBase::TSeed Seed = TBase::DefaultSeed()) noexcept : TBase{Seed} {}

        template <class T = UINT>
        constexpr T Next() noexcept
        {
            if constexpr (sizeof(T) == 64)
                return (Next32() << 32) | Next32();
            else
                return (T)Next32();
        }
        template <class T>
        constexpr T NextInt(T Min, T Max) noexcept
        {
            using TUnsigned = std::make_unsigned_t<T>;
            using TLimits = std::numeric_limits<TUnsigned>;

            const auto Span = (TUnsigned)Max - (TUnsigned)Min + 1;
            const auto Limit = TLimits::max() - (TLimits::max() % Span);
            TUnsigned r;
            do
            {
                r = Next<TUnsigned>();
            } while (r >= Limit);
            return Min + T(r % Span);
        }
        template <class T>
        constexpr T NextFloat(T Min, T Max) noexcept
        {
            if constexpr (std::is_same_v<T, float>)
            {
                const auto r = Next32() >> 8;
                return Min + (r / 16777216.f /* 2^24 */) * (Max - Min);
            }
            else
            {
                const auto r = ((UINT64)Next32() << 21) ^ (Next32() >> 11);
                return Min + (r / 9007199254740992. /* 2^53 */) * (Max - Min);
            }
        }
    };

    static Random<RmPcg32> Generator;
    static SrwLock GeneratorLock;

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
        SrwExclusiveGuard _{GeneratorLock};
        return Generator.NextInt(min, max);
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
        SrwExclusiveGuard _{GeneratorLock};
        return Generator.NextFloat(min, max);
    }
}
