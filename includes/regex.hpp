#pragma once

#include "re2.h"
#pragma once

#include "re2.h"

#include "string.hpp"

namespace pxd {
bool full_match(String &full_str, const RE2 &regex);
bool partial_match(String &full_str, const RE2 &regex);
void replace_first(const RE2 &regex, String &base_str, String &new_str);
void replace_first(const RE2 &regex, String &base_str, const char *new_str);
} // namespace pxd
