#pragma once

#ifdef PXD_USE_STD_STRING
#include <string>
#else
#include "../third-party/SIMDString/SIMDString.h"
#endif

namespace pxd {

class String {
public:
  String() = default;
  String(const std::string &str);
  String(const char *c_str);
  String(const String &other) = default;
  String &operator=(const String &other);
  String &operator=(const std::string &other);
  String(String &&other) = default;
  String &operator=(String &&other);
  String &operator=(std::string &&other);
  String &operator=(const char *other);
  ~String() = default;

  decltype(auto) operator[](int index) { return value[index]; }
  inline decltype(auto) get_value() const { return value; }

  String operator+(const String &other);
  String operator+(String &&other);
  String operator+(const std::string &other);
  String operator+(std::string &&other);
  String operator+(const char *other);

  inline String operator-(const String &other) {
    return String(c_str()).replace_all(other.c_str(), "");
  }

  inline String operator-(String &&other) {
    return String(c_str()).replace_all(other, "");
  }

  inline String operator-(const std::string &other) {
    return String(c_str()).replace_all(other.c_str(), "");
  }

  inline String operator-(std::string &&other) {
    return String(c_str()).replace_all(other.c_str(), "");
  }

  inline String operator-(const char *other) {
    return String(c_str()).replace_all(other, "");
  }

  inline String &operator+=(const String &other) {
    value += other.get_value();
    return *this;
  }
  inline String &operator+=(String &&other) {
    value += other.get_value();
    return *this;
  }
  inline String &operator+=(const std::string &other) {
    value += other;
    return *this;
  }
  inline String &operator+=(std::string &&other) {
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
    return std::string_view(c_str());
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

  /// @brief get string which value is in the center and surrounded by the
  /// fill_char
  /// @param total_length total length of the string
  /// @param fill_char the char which is used to fill the empty spaces
  /// @return
  String &center(int total_length, const char fill_char);

  /// @brief get std::string of the value
  /// @return std::string of the value
  constexpr inline std::string string() const {
#ifdef PXD_USE_STD_STRING
    return value;
#else
    return std::string(value.c_str());
#endif
  }

  /// @brief get string_view of the value
  /// @return string_view of the value
  inline std::string_view string_view() const {
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
  constexpr inline size_t length() const { return value.length(); }

  /// @brief get the const char* of the value
  /// @return const char* representation of the value
  constexpr inline const char *c_str() const { return value.c_str(); }

private:
  constexpr inline bool compare_str(const std::string &str) {
    return value.compare(str);
  }

  constexpr inline bool compare_str(std::string &&str) {
    return value.compare(str);
  }

  constexpr inline bool compare_cstr(const char *cstr) {
#ifdef PXD_USE_STD_STRING
    return value.compare(std::string(cstr));
#else
    return value.compare(cstr);
#endif
  }

private:
#ifdef PXD_USE_STD_STRING
  std::string value;
#else
  SIMDString<64> value;
#endif
};

String operator+(const String &self, const String &other);
String operator+(const String &self, String &&other);
String operator+(const String &self, const std::string &other);
String operator+(const String &self, std::string &&other);
String operator+(const String &self, const char *other);

inline String operator-(const String &self, const String &other) {
  return String(self.c_str()).replace_all(other.c_str(), "");
}

inline String operator-(const String &self, String &&other) {
  return String(self.c_str()).replace_all(other, "");
}

inline String operator-(const String &self, const std::string &other) {
  return String(self.c_str()).replace_all(other.c_str(), "");
}

inline String operator-(const String &self, std::string &&other) {
  return String(self.c_str()).replace_all(other.c_str(), "");
}

inline String operator-(const String &self, const char *other) {
  return String(self.c_str()).replace_all(other, "");
}

inline String to_string(const char *char_arr) { return String(char_arr); }
} // namespace pxd