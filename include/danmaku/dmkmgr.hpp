#include "dmkitem.hpp"

#include <vector>

namespace danmaku
{
    class danmakuManager
    {
    private:
        constexpr static size_t InvalidTrack = std::numeric_limits<size_t>::max();

        struct Track
        {
            float y{};
            std::vector<danmakuItem> items{};
        };

        float screenWidth_{};
        float screenHeight_{};
        float lineHeight_{};
        float lineGap_{};
        float duration_{};
        float itemGap_{};
        float speedFactor_{};
        std::vector<Track> tracks_{};

        // 从上到下寻找第一个合适轨道
        size_t findBestTrack(float itemSpeed) const;

    public:
        // WARNING 尺寸、行高、行距改变后必须调用本函数
        void recalculateTracks();

        bool addDanmaku(danmakuItem &&item);

        // dt单位为秒
        void tick(float dt);

        Gdiplus::Status draw(Gdiplus::GpGraphics *g);

        void setScreenSize(float width, float height)
        {
            screenWidth_ = width;
            screenHeight_ = height;
        }
        void setLineHeight(float lineHeight) { lineHeight_ = lineHeight; }
        void setLineGap(float lineGap) { lineGap_ = lineGap; }
        void setDuration(float duration) { duration_ = duration; }
        void setItemGap(float gap) { itemGap_ = gap; }
        void setSpeedFactor(float factor) { speedFactor_ = factor; }
    };
}