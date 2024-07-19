#include "string.hpp"

namespace pxd {
String::String(const std::string &str) { value = str; }

String::String(const char *c_str) {
#ifdef PXD_USE_STD_STRING
  value = std::string(c_str);
#else
  value = SIMDString<64>(c_str);
#endif
}

String &String::operator=(const std::string &other) {
  value = other;

  return *this;
}

String &String::operator=(std::string &&other) {
  value = std::move(other);

  return *this;
}

String &String::operator=(const char *other) {
  value = other;

  return *this;
}

} // namespace pxd