#pragma once

#ifndef PXD_USE_STD_STRING
#include "../third-party/SIMDString/SIMDString.h"
constexpr int PXD_SIMDSTRING_ALIGNMENT = 64;
#endif

#include "absl/str_cat.hpp"
#include "absl/str_join.hpp"
#include <array>

#include <string_view>

namespace pxd {

class String {
public:
  String() = default;
  String(const std::string &str);
  String(const char *c_str);
  String(const String &other) = default;

#ifndef PXD_USE_STD_STRING
  String(const SIMDString<PXD_SIMDSTRING_ALIGNMENT> &other);
#endif

  auto operator=(const String &other) -> String &;
  auto operator=(const std::string &other) -> String &;
  String(String &&other) noexcept = default;

  auto operator=(String &&other) noexcept -> String &;
  auto operator=(std::string &&other) noexcept -> String &;
  auto operator=(const char *other) -> String &;

  ~String() = default;

  decltype(auto) operator[](int index) { return value[index]; }
  decltype(auto) get_value() const { return value; }

  auto operator+(const String &other) -> String;
  auto operator+(String &&other) -> String;
  auto operator+(const std::string &other) -> String;
  auto operator+(std::string &&other) -> String;
  auto operator+(const char *other) -> String;

  auto operator-(const String &other) -> String;
  auto operator-(String &&other) -> String;
  auto operator-(const std::string &other) -> String;
  auto operator-(std::string &&other) -> String;
  auto operator-(const char *other) -> String;

  auto operator+=(const String &other) -> String &;
  auto operator+=(String &&other) -> String &;
  auto operator+=(const std::string &other) -> String &;
  auto operator+=(std::string &&other) -> String &;
  auto operator+=(const char *other) -> String &;

  auto operator-=(const String &other) -> String &;
  auto operator-=(const std::string &other) -> String &;
  auto operator-=(const char *other) -> String &;

  auto operator==(const char *c_str) -> bool { return compare_cstr(c_str); }
  auto operator==(String &other) -> bool { return value == other.get_value(); }
  auto operator==(String &&other) -> bool {
    return value == std::forward<String>(other).get_value();
  }
  auto operator==(const std::string &str) -> bool { return compare_str(str); }
  auto operator==(std::string &&str) -> bool {
    return compare_str(std::forward<std::string>(str));
  }

  auto operator!=(const char *c_str) -> bool { return !compare_cstr(c_str); }
  auto operator!=(String &other) -> bool { return value != other.get_value(); }
  auto operator!=(String &&other) -> bool {
    return value != std::forward<String>(other).get_value();
  }
  auto operator!=(const std::string &str) -> bool { return !compare_str(str); }
  auto operator!=(std::string &&str) -> bool {
    return !compare_str(std::forward<std::string>(str));
  }

  /// @brief return as std::string
  operator std::string() { return string(); }

  /// @brief return as std::string_view
  operator std::string_view() { return std::string_view(c_str()); }

  /// @brief return as const char*
  operator const char *() { return c_str(); }

  // -----------------------------------------------------------------------------
  // -- Constant Strings

  static decltype(auto) get_punctuation() {
    return std::forward<String>(String("!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~"));
  }

  static decltype(auto) get_digits() {
    return std::forward<String>(String("0123456789"));
  }

  static decltype(auto) get_hex_digits() {
    return std::forward<String>(String("0123456789abcdefABCDEF"));
  }

  static decltype(auto) get_oct_digits() {
    return std::forward<String>(String("01234567"));
  }

  static decltype(auto) get_ascii_lowers() {
    return std::forward<String>(String("abcdefghijklmnopqrstuvwxyz"));
  }

  static decltype(auto) get_ascii_uppers() {
    return std::forward<String>(String("ABCDEFGHIJKLMNOPQRSTUVWXYZ"));
  }

  /// @brief get string which value is in the center and surrounded by the
  /// fill_char
  /// @param total_length total length of the string
  /// @param fill_char the char which is used to fill the empty spaces
  /// @return
  auto center(int total_length, const char fill_char) -> String &;

  /// @brief get std::string of the value
  /// @return std::string of the value
  auto string() const -> std::string {
#ifdef PXD_USE_STD_STRING
    return value;
#else
    return std::string(value.c_str());
#endif
  }

  /// @brief get string_view of the value
  /// @return string_view of the value
  auto string_view() const -> std::string_view {
    return std::string_view(value.c_str());
  }

  /// @brief get the const char* of the value
  /// @return const char* representation of the value
  auto c_str() const -> const char * { return value.c_str(); }

  /// @brief replace the first occurence of the old_val with the new_val
  /// @param old_val wanted to be replaced
  /// @param new_val wanted to be replaced with
  auto replace_first(const char *old_val, const char *new_val) -> String &;

  /// @brief replace the all occurences of the old_val with the new_val
  /// @param old_val wanted to be replaced
  /// @param new_val wanted to be replaced with
  auto replace_all(const char *old_val, const char *new_val) -> String &;

  /// @brief get the length of the value
  /// @return length of the value
  auto length() const -> size_t { return value.length(); }

  template <typename H> friend H AbslHashValue(H hasher, const String &str) {
    return H::combine(std::move(hasher), str.string());
  }

private:
  auto compare_str(const std::string &str) -> bool {
    return value.compare(str) == 0;
  }

  auto compare_str(std::string &&str) -> bool {
    return value.compare(std::forward<std::string>(str)) == 0;
  }

  auto compare_cstr(const char *cstr) -> bool {
#ifdef PXD_USE_STD_STRING
    return value.compare(std::string(cstr)) == 0;
#else
    return value.compare(cstr) == 0;
#endif
  }

private:
#ifdef PXD_USE_STD_STRING
  std::string value = {};
#else
  SIMDString<PXD_SIMDSTRING_ALIGNMENT> value = {};
#endif
};

auto operator+(const String &self, const String &other) -> String;
auto operator+(const String &self, String &&other) -> String;
auto operator+(const String &self, const std::string &other) -> String;
auto operator+(const String &self, std::string &&other) -> String;
auto operator+(const String &self, const char *other) -> String;

auto operator==(const String &self, const String &other) -> bool;
auto operator==(const String &self, String &&other) -> bool;
auto operator==(const String &self, const std::string &other) -> bool;
auto operator==(const String &self, std::string &&other) -> bool;
auto operator==(const String &self, const char *other) -> bool;

auto operator!=(const String &self, const String &other) -> bool;
auto operator!=(const String &self, String &&other) -> bool;
auto operator!=(const String &self, const std::string &other) -> bool;
auto operator!=(const String &self, std::string &&other) -> bool;
auto operator!=(const String &self, const char *other) -> bool;

namespace str {
auto to_string(const char *char_arr) -> String;

template <typename... Str>
static auto join(const char *seperator, const Str &...given_values) -> String {
  const size_t given_values_count = sizeof...(given_values);
  std::array<absl::string_view, given_values_count> values({given_values...});

  return absl::StrJoin(values, seperator);
}

} // namespace str

} // namespace pxd