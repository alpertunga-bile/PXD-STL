#pragma once

#include "SIMDString.h"

namespace pxd {

class String {
public:
  String() = default;
  String(const std::string &str);
  String(const char *c_str);
  String(const String &other) = default;
  String &operator=(const String &other) = default;
  String &operator=(const std::string &other);
  String(String &&other) = default;
  String &operator=(String &&other) = default;
  String &operator=(std::string &&other);
  String &operator=(const char *other);
  ~String() = default;

  constexpr inline bool operator==(const char *c_str) {
#ifdef PXD_USE_STD_STRING
    return value == std::string(c_str);
#else
    return value == c_str;
#endif
  }

  constexpr inline bool operator==(String &other) {
    return value == other.get_value();
  }

  constexpr inline bool operator==(String &&other) {
    return value == other.get_value();
  }

  constexpr inline bool operator==(const std::string &str) {
#ifdef PXD_USE_STD_STRING
    return value.compare(str);
#else
    return value == str;
#endif
  }

  constexpr inline bool operator==(std::string &&str) {
#ifdef PXD_USE_STD_STRING
    return value.compare(str);
#else
    return value == str;
#endif
  }

#ifdef PXD_USE_STD_STRING
  std::string get_value() { return value; }
  std::string get_value() const { return value; }
#else
  SIMDString<64> get_value() { return value; }
  SIMDString<64> get_value() const { return value; }
#endif

  size_t length() { return value.length(); }

  decltype(auto) c_str() { return value.c_str(); }

private:
#ifdef PXD_USE_STD_STRING
  std::string value;
#else
  SIMDString<64> value;
#endif
};

inline String to_string(const char *char_arr) { return String(char_arr); }
} // namespace pxd