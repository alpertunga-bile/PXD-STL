#pragma once

#ifdef PXD_USE_STD_STRING
#include <string>
#else
#include "SIMDString.h"
#endif

namespace pxd {

class String {
public:
  String(const char *c_str);

  constexpr inline bool operator==(const char *c_str) {
#ifdef PXD_USE_STD_STRING
    return value == std::string(c_str);
#else
    return value == c_str;
#endif
  }
  constexpr inline bool operator==(std::string str) { return value == str; }

#ifdef PXD_USE_STD_STRING
  std::string get_value() { return value; }
#else
  SIMDString<64> get_value() { return value; }
#endif

private:
#ifdef PXD_USE_STD_STRING
  std::string value;
#else
  SIMDString<64> value;
#endif
};

inline String to_string(const char *char_arr) { return String(char_arr); }
} // namespace pxd