#include "pch.hpp"
#include "danmaku/dmkitem.hpp"

namespace danmaku
{
  /**
   * @brief 从 JSON 文件中读取弹幕数组
   * @param filename JSON 文件路径（宽字符串）
   * @return 包含 DanmakuItem 对象的动态数组
   */
  std::vector<DanmakuItem> ReadDanmakuArrayFromJsonFile(const std::wstring &filename);
}