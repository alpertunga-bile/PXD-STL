#include "string.hpp"

#include "format.h"
#include "regex.hpp"

namespace pxd {
String::String(const std::string &str) { value = str; }

String::String(const char *c_str) {
#ifdef PXD_USE_STD_STRING
  value = std::string(c_str);
#else
  value = SIMDString<PXD_SIMDSTRING_ALIGNMENT>(c_str);
#endif
}

String::String(const SIMDString<PXD_SIMDSTRING_ALIGNMENT> &other) {
  value = other;
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

String::String(SIMDString<PXD_SIMDSTRING_ALIGNMENT> &&other) {
  value = std::forward<SIMDString<PXD_SIMDSTRING_ALIGNMENT>>(other);
}

auto String::operator=(String &&other) noexcept -> String & {
  value = std::move(std::forward<String>(other).get_value());

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
  return absl::StrCat(value, other.get_value());
#endif
  return value + other.get_value();
#endif
}

auto String::operator+(String &&other) -> String {
#ifdef PXD_USE_STD_STRING
  return abls::StrCat(value, std::forward<String>(other).get_value());
#else
  return value + std::forward<String>(other).get_value();
#endif
}

auto String::operator+(const std::string &other) -> String {
#ifdef PXD_USE_STD_STRING
  return absl::StrCat(value + other);
#else
  return value + other;
#endif
}

auto String::operator+(std::string &&other) -> String {
#ifdef PXD_USE_STD_STRING
  return absl::StrCat(value + std::forward<std::string>(other));
#else
  return value + std::forward<std::string>(other);
#endif
}

auto String::operator+(const char *other) -> String {
#ifdef PXD_USE_STD_STRING
  return absl::StrCat(value, other);
#else
  return value + other;
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
  return self + other;
}

auto operator+(const String &self, String &&other) -> String {
  return self + std::forward<String>(other);
}

auto operator+(const String &self, const std::string &other) -> String {
  return self + other;
}

auto operator+(const String &self, std::string &&other) -> String {
  return self + std::forward<std::string>(other);
}

auto operator+(const String &self, const char *other) -> String {
  return self + other;
}

namespace str {
auto to_string(const char *char_arr) -> String { return String(char_arr); }

} // namespace str

} // namespace pxd