#include "string.hpp"

namespace pxd {
String::String(const char *c_str) {
#ifdef PXD_USE_STD_STRING
  value = std::string(c_str);
#else
  value = SIMDString<64>(c_str);
#endif
}

} // namespace pxd