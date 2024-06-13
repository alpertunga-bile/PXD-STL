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
} // namespace pxd