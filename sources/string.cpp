#include "string.hpp"

#include "format.h"
#include "regex.hpp"

namespace pxd {
String::String(const std::string &str) { value = str; }

String::String(const char *c_str) {
#ifdef PXD_USE_STD_STRING
  value = std::string(c_str);
#else
  value = SIMDString<64>(c_str);
#endif
}

String &String::operator=(const String &other) {
#ifdef PXD_USE_STD_STRING
  value = other.get_value();
#else
  value = other.c_str();
#endif

  return *this;
}

String &String::operator=(const std::string &other) {
  value = other;

  return *this;
}

String &String::operator=(String &&other) {
  value = std::move(other.string());

  return *this;
}

String &String::operator=(std::string &&other) {
  value = std::move(other);

  return *this;
}

String &String::operator=(const char *other) {
  value = other;

  return *this;
}

String String::operator+(const String &other) {
#ifdef PXD_USE_STD_STRING
  return String(value + other.get_value());
#else
  return String((value + other.get_value()).c_str());
#endif
}

String String::operator+(String &&other) {
#ifdef PXD_USE_STD_STRING
  return String(value + other.get_value());
#else
  return String((value + other.get_value()).c_str());
#endif
}

String String::operator+(const std::string &other) {
#ifdef PXD_USE_STD_STRING
  return String(value + other);
#else
  return String((value + other.c_str()).c_str());
#endif
}

String String::operator+(std::string &&other) {
#ifdef PXD_USE_STD_STRING
  return String(value + other);
#else
  return String((value + other.c_str()).c_str());
#endif
}

String String::operator+(const char *other) {
#ifdef PXD_USE_STD_STRING
  return String(value + other);
#else
  return String((value + other).c_str());
#endif
}

String &String::center(int total_length, const char fill_char) {
  auto format_string = fmt::format("{{:{}^{}}}", fill_char, total_length);
  auto format_args = fmt::make_format_args(value);

  value = fmt::vformat(format_string.c_str(), format_args);

  return *this;
}

inline String &String::replace_first(const char *old_val, const char *new_val) {
  RE2 regex(fmt::format(".*(\\b{}\\b).*", old_val));

  if (!check_regex(regex)) {
    return *this;
  }

#ifdef PXD_USE_STD_STRING
  RE2::Replace(&value, regex, new_val);
#else
  std::string temp_str = string();
  RE2::Replace(&temp_str, regex, new_val);
  value = std::forward<std::string>(temp_str);
#endif

  return *this;
}

inline String &String::replace_all(const char *old_val, const char *new_val) {
  RE2 regex(fmt::format(".*(\\b{}\\b).*", old_val));

  if (!check_regex(regex)) {
    return *this;
  }

#ifdef PXD_USE_STD_STRING
  RE2::GlobalReplace(&value, regex, new_val);
#else
  std::string temp_str = string();
  RE2::GlobalReplace(&temp_str, regex, new_val);
  value = std::forward<std::string>(temp_str);
#endif

  return *this;
}

String operator+(const String &self, const String &other) {
#ifdef PXD_USE_STD_STRING
  return String(self.get_value() + other.get_value());
#else
  return String((self.get_value() + other.get_value()).c_str());
#endif
}

String operator+(const String &self, String &&other) {
#ifdef PXD_USE_STD_STRING
  return String(self.get_value() + other.get_value());
#else
  return String((self.get_value() + other.get_value()).c_str());
#endif
}

String operator+(const String &self, const std::string &other) {
#ifdef PXD_USE_STD_STRING
  return String(self.get_value() + other);
#else
  return String((self.get_value() + other.c_str()).c_str());
#endif
}

String operator+(const String &self, std::string &&other) {
#ifdef PXD_USE_STD_STRING
  return String(self.get_value() + other);
#else
  return String((self.get_value() + other.c_str()).c_str());
#endif
}

String operator+(const String &self, const char *other) {
#ifdef PXD_USE_STD_STRING
  return String(self.get_value() + other);
#else
  return String((self.get_value() + other).c_str());
#endif
}

} // namespace pxd