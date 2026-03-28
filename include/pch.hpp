#ifndef PCH_HPP
#define PCH_HPP

#define _WIN32_IE 0x0600
#define WINVER 0x0600
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0600
#endif

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
#include <sstream>

#include <assert.h>

#include "functions/gpptr.hpp"
#include "functions/srwlk.hpp"
#include "functions/files.hpp"
#include "functions/str.hpp"

using namespace Gdiplus::DllExports;

#endif // PCH_HPP