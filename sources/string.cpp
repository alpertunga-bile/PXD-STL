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

auto String::operator=(const String &other) -> String & {
#ifdef PXD_USE_STD_STRING
  value = other.get_value();
#else
  value = other.c_str();
#endif

  return *this;
}

auto String::operator=(const std::string &other) -> String & {
  value = other;

  return *this;
}

auto String::operator=(String &&other) noexcept -> String & {
  value = std::move(other.string());

  return *this;
}

auto String::operator=(std::string &&other) noexcept -> String & {
  value = std::move(other);

  return *this;
}

auto String::operator=(const char *other) -> String & {
  value = other;

  return *this;
}

auto String::operator+(const String &other) -> String {
#ifdef PXD_USE_STD_STRING
  return String(value + other.get_value());
#else
  return String((value + other.get_value()).c_str());
#endif
}

auto String::operator+(String &&other) -> String {
#ifdef PXD_USE_STD_STRING
  return String(value + other.get_value());
#else
  return String((value + other.get_value()).c_str());
#endif
}

auto String::operator+(const std::string &other) -> String {
#ifdef PXD_USE_STD_STRING
  return String(value + other);
#else
  return String((value + other.c_str()).c_str());
#endif
}

auto String::operator+(std::string &&other) -> String {
#ifdef PXD_USE_STD_STRING
  return String(value + other);
#else
  return String((value + other.c_str()).c_str());
#endif
}

auto String::operator+(const char *other) -> String {
#ifdef PXD_USE_STD_STRING
  return String(value + other);
#else
  return String((value + other).c_str());
#endif
}

auto String::center(int total_length, const char fill_char) -> String & {
  auto format_string = fmt::format("{{:{}^{}}}", fill_char, total_length);
  auto format_args = fmt::make_format_args(value);

  value = fmt::vformat(format_string.c_str(), format_args);

  return *this;
}

auto String::replace_first(const char *old_val,
                           const char *new_val) -> String & {
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

auto String::replace_all(const char *old_val, const char *new_val) -> String & {
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

auto operator+(const String &self, const String &other) -> String {
#ifdef PXD_USE_STD_STRING
  return String(self.get_value() + other.get_value());
#else
  return String((self.get_value() + other.get_value()).c_str());
#endif
}

auto operator+(const String &self, String &&other) -> String {
#ifdef PXD_USE_STD_STRING
  return String(self.get_value() + other.get_value());
#else
  return String((self.get_value() + other.get_value()).c_str());
#endif
}

auto operator+(const String &self, const std::string &other) -> String {
#ifdef PXD_USE_STD_STRING
  return String(self.get_value() + other);
#else
  return String((self.get_value() + other.c_str()).c_str());
#endif
}

auto operator+(const String &self, std::string &&other) -> String {
#ifdef PXD_USE_STD_STRING
  return String(self.get_value() + other);
#else
  return String((self.get_value() + other.c_str()).c_str());
#endif
}

auto operator+(const String &self, const char *other) -> String {
#ifdef PXD_USE_STD_STRING
  return String(self.get_value() + other);
#else
  return String((self.get_value() + other).c_str());
#endif
}

auto to_string(const char *char_arr) -> String { return String(char_arr); }

} // namespace pxd