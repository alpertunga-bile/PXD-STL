#pragma once

#include "SIMDString.h"

namespace pxd {

class String {
public:
  String() = default;
  String(const SIMDString<64> &str);
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

  decltype(auto) operator[](int index) { return value[index]; }

  inline decltype(auto) operator+(const String &other) {
    return std::forward<String>(String(value + other.get_value()));
  }
  inline decltype(auto) operator+(const std::string &other) {
    return std::forward<String>(String(string() + other));
  }
  inline decltype(auto) operator+(const char *other) {
    return std::forward<String>(String(string() + std::string(other)));
  }

  inline decltype(auto) operator-(String &other) {
    return std::forward<String>(String(value).replace_all(other, ""));
  }

  inline decltype(auto) operator-(std::string &other) {
    return std::forward<String>(String(value).replace_all(other.c_str(), ""));
  }

  inline decltype(auto) operator-(const char *other) {
    return std::forward<String>(String(value).replace_all(other, ""));
  }

  inline String &operator+=(String &other) {
    value += other.get_value();
    return *this;
  }
  inline String &operator+=(std::string &other) {
    value += other;
    return *this;
  }
  inline String &operator+=(const char *other) {
    value += other;
    return *this;
  }

  inline String &operator-=(String &other) { return replace_all(other, ""); }
  inline String &operator-=(std::string &other) {
    return replace_all(other.c_str(), "");
  }
  inline String &operator-=(const char *other) {
    return replace_all(other, "");
  }

  constexpr inline bool operator==(const char *c_str) {
    return compare_cstr(c_str);
  }

  constexpr inline bool operator==(String &other) {
    return value == other.get_value();
  }

  constexpr inline bool operator==(String &&other) {
    return value == other.get_value();
  }

  constexpr inline bool operator==(const std::string &str) {
    return compare_str(str);
  }

  constexpr inline bool operator==(std::string &&str) {
    return compare_str(str);
  }

  constexpr inline bool operator!=(const char *c_str) {
    return !compare_cstr(c_str);
  }

  constexpr inline bool operator!=(String &other) {
    return value != other.get_value();
  }

  constexpr inline bool operator!=(String &&other) {
    return value != other.get_value();
  }

  constexpr inline bool operator!=(const std::string &str) {
    return !compare_str(str);
  }

  constexpr inline bool operator!=(std::string &&str) {
    return !compare_str(str);
  }

  /// @brief return as std::string
  constexpr inline operator std::string() { return string(); }

  /// @brief return as std::string_view
  constexpr inline operator std::string_view() {
    return std::string_view(value.c_str());
  }

  /// @brief return as const char*
  constexpr inline operator const char *() { return c_str(); }

  // -----------------------------------------------------------------------------
  // -- Constant Strings

  static constexpr inline decltype(auto) get_punctuation() {
    return std::forward<String>(String("!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~"));
  }

  static constexpr inline decltype(auto) get_digits() {
    return std::forward<String>(String("0123456789"));
  }

  static constexpr inline decltype(auto) get_hex_digits() {
    return std::forward<String>(String("0123456789abcdefABCDEF"));
  }

  static constexpr inline decltype(auto) get_oct_digits() {
    return std::forward<String>(String("01234567"));
  }

  static constexpr inline decltype(auto) get_ascii_lowers() {
    return std::forward<String>(String("abcdefghijklmnopqrstuvwxyz"));
  }

  static constexpr inline decltype(auto) get_ascii_uppers() {
    return std::forward<String>(String("ABCDEFGHIJKLMNOPQRSTUVWXYZ"));
  }

#ifdef PXD_USE_STD_STRING
  inline std::string get_value() const { return value; }
  /// @brief get std::string of the value
  /// @return std::string of the value
  constexpr inline std::string string() { return value; }
#else
  inline SIMDString<64> get_value() const { return value; }
  /// @brief get std::string of the value
  /// @return std::string of the value
  constexpr inline std::string string() { return std::string(value.c_str()); }
#endif

  /// @brief get string_view of the value
  /// @return string_view of the value
  inline std::string_view string_view() {
    return std::string_view(value.c_str());
  }

  /// @brief replace the first occurence of the old_val with the new_val
  /// @param old_val wanted to be replaced
  /// @param new_val wanted to be replaced with
  inline String &replace_first(const char *old_val, const char *new_val);

  /// @brief replace the all occurences of the old_val with the new_val
  /// @param old_val wanted to be replaced
  /// @param new_val wanted to be replaced with
  inline String &replace_all(const char *old_val, const char *new_val);

  /// @brief get the length of the value
  /// @return length of the value
  constexpr inline size_t length() { return value.length(); }

  /// @brief get the const char* of the value
  /// @return const char* representation of the value
  constexpr inline const char *c_str() { return value.c_str(); }

private:
  constexpr inline bool compare_str(const std::string &str) {
#ifdef PXD_USE_STD_STRING
    return value.compare(str);
#else
    return value == str;
#endif
  }

  constexpr inline bool compare_str(std::string &&str) {
#ifdef PXD_USE_STD_STRING
    return value.compare(str);
#else
    return value == str;
#endif
  }

  constexpr inline bool compare_cstr(const char *cstr) {
#ifdef PXD_USE_STD_STRING
    return value.compare(std::string(cstr));
#else
    return value == cstr;
#endif
  }

private:
#ifdef PXD_USE_STD_STRING
  std::string value;
#else
  SIMDString<64> value;
#endif
};

inline String to_string(const char *char_arr) { return String(char_arr); }
} // namespace pxd