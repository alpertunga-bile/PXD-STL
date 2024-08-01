#pragma once

#include "../third-party/re2/re2/re2.h"

namespace pxd {

class String;

auto check_regex(const RE2 &regex) -> bool;

template <typename Func, typename Str, typename... A>
inline auto do_it(Func func, Str str, const RE2 &regex,
                  const A &...kwargs) -> bool {
  const RE2::Arg *const args[] = {&kwargs...};
  const int n = sizeof...(kwargs);
  return func(str, regex, args, n);
}

template <typename Func, typename Str>
inline auto do_it(Func func, Str str, const RE2 &regex) -> bool {
  return func(str, regex, NULL, 0);
}

template <typename T, typename... A>
auto full_match(T &full_str, const RE2 &regex, A &&...kwargs) -> bool {
  if (!check_regex(regex)) {
    return false;
  }

  return do_it(RE2::FullMatchN, full_str.c_str(), regex,
               RE2::Arg(std::forward<A>(kwargs))...);
}

template <typename T, typename... A>
auto partial_match(T &full_str, const RE2 &regex, A &&...kwargs) -> bool {
  if (!check_regex(regex)) {
    return false;
  }

  return do_it(RE2::PartialMatchN, full_str.c_str(), regex,
               RE2::Arg(std::forward<A>(kwargs))...);
}

template <typename T, typename... A>
auto loop_it(T *input, const RE2 &regex, A &&...kwargs) -> bool {
  if (!check_regex(regex)) {
    return false;
  }

  return do_it(RE2::ConsumeN, input, regex, Arg(std::forward<A>(kwargs))...);
}

void replace_first(const RE2 &regex, String &base_str, String &new_str);
void replace_first(const RE2 &regex, String &base_str, String &&new_str);

void replace_all(const RE2 &regex, String &base_str, String &new_str);
void replace_all(const RE2 &regex, String &base_str, String &&new_str);

auto get_escaped_string(String &base_str) -> String;
auto get_escaped_string(String &&base_str) -> String;
} // namespace pxd
