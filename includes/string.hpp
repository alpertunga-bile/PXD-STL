#pragma once

#ifdef PXD_USE_STD_STRING
#include <string>
using String = std::string
#else
#include "SIMDString.h"
using String = SIMDString<64>;
#endif