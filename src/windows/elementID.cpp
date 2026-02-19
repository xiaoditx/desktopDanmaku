#include "windows/elements.hpp"

namespace danmaku
{
    // 静态成员定义及初始化
    UINT_PTR Element::currentElementID = 0;
    std::unordered_map<UINT_PTR, Element *> Element::s_idMap;
    std::vector<UINT_PTR> Element::s_freeIds;
    // 互斥锁（我知道我们这里用不上但我想试试）
    // （毕竟这个代码未来或许能用上）
    std::mutex Element::s_mutex;

    // ---------- 静态辅助函数 ----------
    UINT_PTR Element::allocateID(Element *elem)
    {
        std::lock_guard<std::mutex> lock(s_mutex);
        UINT_PTR id;
        if (!s_freeIds.empty())
        {
            id = s_freeIds.back();
            s_freeIds.pop_back();
        }
        else
        {
            id = ++currentElementID;
        }
        s_idMap[id] = elem;
        return id;
    }

    void Element::releaseID(UINT_PTR id, Element *obj)
    {
        std::lock_guard<std::mutex> lock(s_mutex);
        auto it = s_idMap.find(id); // 迭代器
        // 确保要释放的ID确实对应正确的对象（防止错误调用）
        // 否则可能重复释放或无效ID
        if (it != s_idMap.end() && it->second == obj)
        {
            s_idMap.erase(it);       // 从对照表中移除
            s_freeIds.push_back(id); // 加入空闲池以供重用
        }
        else
        {
            debug::logOutput("[erro] releaseID: invalid ID or mismatched object", L"\n");
        }
    }

    Element &searchID(UINT_PTR id)
    {
        std::lock_guard<std::mutex> lock(Element::s_mutex);
        auto it = Element::s_idMap.find(id);
        if (it != Element::s_idMap.end())
        {
            return *(it->second);
        }
        else
        {
            throw std::runtime_error("Element::searchID: invalid ID");
        }
    }
    // ID所有权转移
    void Element::idTransfer(UINT_PTR id, Element *newOwner)
    {
        std::lock_guard<std::mutex> lock(Element::s_mutex);
        auto it = Element::s_idMap.find(id);
        if (it != Element::s_idMap.end())
        {
            it->second = newOwner;
        }
        else
        {
            throw std::runtime_error("idTransfer: invalid ID");
        }
    }
}