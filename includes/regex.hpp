#pragma once

#include "re2.h"

#include "string.hpp"

namespace pxd {
bool check_regex(const RE2 &regex);

template <typename Func, typename Str, typename... A>
inline bool do_it(Func f, Str str, const RE2 &regex, const A &...a) {
  const RE2::Arg *const args[] = {&a...};
  const int n = sizeof...(a);
  return f(str, regex, args, n);
}

template <typename Func, typename Str>
inline bool do_it(Func f, Str str, const RE2 &regex) {
  return f(str, regex, NULL, 0);
}

template <typename T, typename... A>
bool full_match(T &full_str, const RE2 &regex, A &&...a) {
  if (!check_regex(regex)) {
    return false;
  }

  return do_it(RE2::FullMatchN, full_str.c_str(), regex,
               RE2::Arg(std::forward<A>(a))...);
}

template <typename T, typename... A>
bool partial_match(T &full_str, const RE2 &regex, A &&...a) {
  if (!check_regex(regex)) {
    return false;
  }

  return do_it(RE2::PartialMatchN, full_str.c_str(), regex,
               RE2::Arg(std::forward<A>(a))...);
}

template <typename T, typename... A>
bool loop_it(T *input, const RE2 &regex, A &&...a) {
  if (!check_regex(regex)) {
    return false;
  }

  return do_it(RE2::ConsumeN, input, regex, Arg(std::forward<A>(a))...);
}

void replace_first(const RE2 &regex, String &base_str, String &new_str);
void replace_first(const RE2 &regex, String &base_str, String &&new_str);

void replace_all(const RE2 &regex, String &base_str, String &new_str);
void replace_all(const RE2 &regex, String &base_str, String &&new_str);

String get_escaped_string(String &base_str);
String get_escaped_string(String &&base_str);
} // namespace pxd