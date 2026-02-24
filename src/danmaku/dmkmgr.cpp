#include "pch.hpp"
#include "danmaku/dmkmgr.hpp"

namespace danmaku
{
    // FIXME 充满时仍然应当选择一个轨道显示
    size_t DanmakuManager::findBestTrack(float itemSpeed) const
    {
        constexpr size_t startTrack = 0;
        for (size_t i = 0; i < tracks_.size(); ++i)
        {
            const auto idx = (startTrack + i) % tracks_.size(); // 考虑到非0起始位置
            const auto &track = tracks_[idx];
            if (track.items.empty())
                return idx;
            const auto &lastItem = track.items.back();
            // A = 轨道上最后一条弹幕
            // B = 新增弹幕
            const auto widthA = lastItem.getWidth();
            const auto speedA = lastItem.getSpeed();
            // A必须完全可见
            if (lastItem.getX() + widthA + itemGap_ > screenWidth_)
                continue;
            // 如果B的速度大于A的速度，B可能会在A离开屏幕前追尾
            // 测试选择当前轨道是否会发生追尾，如果是，则轨道不可用
            if (itemSpeed > speedA)
            {
                const auto leaveTimeA = (lastItem.getX() + widthA) / speedA;
                const auto relativeDist = screenWidth_ - (lastItem.getX() + widthA + itemGap_);
                const auto timeToCollision = relativeDist / (itemSpeed - speedA);
                if (timeToCollision < leaveTimeA)
                    continue;
            }
            return idx;
        }
        return InvalidTrack;
    }

    void DanmakuManager::recalculateTracks()
    {
        size_t trackCount = (size_t)ceilf(screenHeight_ / lineHeight_);
        tracks_.resize(trackCount);
        // 更新轨道坐标
        for (size_t i = 0; i < trackCount; ++i)
        {
            tracks_[i].y =
                i * (lineHeight_ + lineGap_) + lineGap_ / 2.f /* 居中对齐 */;
        }
    }

    bool DanmakuManager::addDanmaku(DanmakuItem &&item)
    {
        item.rasterize();
        const auto distance = screenWidth_ + item.getWidth();
        const auto speed = distance / duration_ * speedFactor_;

        const auto idx = findBestTrack(speed);
        if (idx != InvalidTrack)
        {
            maxValidTrack_ = std::max(maxValidTrack_, idx + 1);
            auto &dmk = tracks_[idx].items.emplace_back(std::move(item));
            dmk.setSpeed(speed);
            dmk.setX(screenWidth_); // 起点
            return true;
        }
        return false;
    }

    BOOL DanmakuManager::tick(float dt)
    {
        DanmakuBitmapCache::instance().tick();
        // 本次状态更新完成后的矩形
        RECT newRect{INT_MAX, INT_MAX, INT_MIN, INT_MIN};
        BOOL bUpdated{};
        size_t maxTrack{};
        for (size_t i = 0; i < maxValidTrack_; ++i)
        {
            auto& track = tracks_[i];
            if (!track.items.empty())
            {
                bUpdated = TRUE;
                maxTrack = i + 1;
            }

            for (auto it = track.items.begin(); it != track.items.end();)
            {
                it->setX(it->getX() - it->getSpeed() * dt);
                if (it->getX() + it->getWidth() <= 0.f)
                    it = track.items.erase(it);
                else
                {
                    const auto l = (int)floorf(it->getX());
                    const auto r = (int)ceilf(it->getX() + it->getWidth());
                    const auto t = (int)floorf(track.y);
                    const auto b = (int)ceilf(track.y + it->getHeight());
                    if (l < newRect.left)
                        newRect.left = l;
                    if (r > newRect.right)
                        newRect.right = r;
                    if (t < newRect.top)
                        newRect.top = t;
                    if (b > newRect.bottom)
                        newRect.bottom = b;
                    ++it;
                }
            }
        }
        maxValidTrack_ = maxTrack;
        // 并上一状态的矩形以便擦除
        UnionRect(&dirtyRect_, &dirtyRectLast_, &newRect);
        dirtyRectLast_ = newRect;
        return bUpdated;
    }

    Gdiplus::Status DanmakuManager::drawGp(Gdiplus::GpGraphics *g)
    {
        Gdiplus::Status status = Gdiplus::Ok;
        for (auto &track : tracks_)
        {
            for (auto &item : track.items)
            {
                status = item.drawGp(g, item.getX(), track.y);
                if (status != Gdiplus::Ok)
                    return status;
            }
        }
        return status;
    }

    BOOL DanmakuManager::drawGdi(HDC dcDst, HDC cdc)
    {
        BOOL b;
        for (auto &track : tracks_)
        {
            for (auto &item : track.items)
            {
                b = item.drawGdi(dcDst, cdc, item.getX(), track.y);
                if (!b)
                    return FALSE;
            }
        }
        return TRUE;
    }
}