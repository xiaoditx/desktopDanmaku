#ifndef PCH_HPP
#define PCH_HPP

#define _WIN32_IE 0x0600
#define WINVER 0x0600
#define _WIN32_WINNT 0x0600

#include <windows.h>
#include <commctrl.h>
#include <gdiplus.h>

#include <vector>
#include <string>
#include <optional>
#include <memory>
#include <unordered_map>
#include <mutex>
#include <functional>
#include <fstream>
#include <codecvt>

#include <assert.h>

#include "functions/gpptr.hpp"
#include "functions/srwlk.hpp"
#include "functions/files.hpp"

using namespace Gdiplus::DllExports;

#endif // PCH_HPP