#include "regex.hpp"

#include "string.hpp"

#include "logger.hpp"

namespace pxd {
auto check_regex(const RE2 &regex) -> bool {
  if (!regex.ok()) {
    PXD_LOG_ERROR(regex.error().c_str());
    return false;
  }

  return true;
}

void replace_first(const RE2 &regex, String &base_str, String &new_str) {
  if (!check_regex(regex)) {
    return;
  }

#ifdef PXD_USE_STD_STRING
  RE2::Replace(&base_str, regex, new_str);
#else
  std::string temp_str = base_str;
  RE2::Replace(&temp_str, regex, new_str);
  base_str = std::forward<std::string>(temp_str);
#endif
}

void replace_first(const RE2 &regex, String &base_str, String &&new_str) {
  replace_first(regex, base_str, new_str);
}

void replace_all(const RE2 &regex, String &base_str, String &new_str) {
  if (!check_regex(regex)) {
    return;
  }

#ifdef PXD_USE_STD_STRING
  RE2::GlobalReplace(&base_str, regex, new_str);
#else
  std::string temp_str(base_str.c_str());
  RE2::GlobalReplace(&temp_str, regex, new_str);
  base_str = std::forward<std::string>(temp_str);
#endif
}

void replace_all(const RE2 &regex, String &base_str, String &&new_str) {
  replace_all(regex, base_str, new_str);
}

auto get_escaped_string(String &base_str) -> String {
#ifdef PXD_USE_STD_STRING
  return RE2::QuoteMeta(base_str.c_str());
#else
  return String(RE2::QuoteMeta(base_str.c_str()));
#endif
}

auto get_escaped_string(String &&base_str) -> String {
  return get_escaped_string(base_str);
}
} // namespace pxd
