#include "regex.hpp"

#include "logger.hpp"

namespace pxd {
bool full_match(String &full_str, const RE2 &regex) {
  if (!regex.ok()) {
    PXD_LOG_ERROR(regex.error().c_str());
  }

  return RE2::FullMatch(full_str.c_str(), regex);
}

bool partial_match(String &full_str, const RE2 &regex) {
  if (!regex.ok()) {
    PXD_LOG_ERROR(regex.error().c_str());
  }

  return RE2::PartialMatch(full_str.c_str(), regex);
}

void replace_first(const RE2 &regex, String &base_str, String &new_str) {
  if (!regex.ok()) {
    PXD_LOG_ERROR(regex.error().c_str());
  }

  RE2::Replace(base_str, regex, new_str.c_str());
}
} // namespace pxd