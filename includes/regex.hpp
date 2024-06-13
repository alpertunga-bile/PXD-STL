#pragma once

#include "re2.h"

#include "string.hpp"

namespace pxd {
bool full_match(String &full_str, const RE2 &regex);
bool partial_match(String &full_str, const RE2 &regex);
} // namespace pxd