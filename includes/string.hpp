#pragma once

#ifdef PXD_USE_STD_STRING
#include <string>
using String = std::string;
#else
#include "SIMDString.h"
using String = SIMDString<64>;
#endif

namespace pxd {
inline String to_string(const char *char_arr) { return String(char_arr); }
} // namespace pxd