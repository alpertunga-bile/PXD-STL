#pragma once

#ifndef RAPIDJSON_SSE2
#define RAPIDJSON_SSE2
#endif

#include "string.hpp"

#include "../third-party/rapidjson/include/rapidjson/rapidjson.h"

#include "../third-party/rapidjson/include/rapidjson/document.h"

#include "../third-party/blake3/c/blake3.h"

#include <array>

namespace pxd {

struct Json {
  String filepath = {};
  std::array<uint8_t, BLAKE3_OUT_LEN> content_hash = {};
  rapidjson::Document document = {};
  rapidjson::UTFType utf_type = {};
};

auto load_json(String filepath) -> Json;

void write_json(String filepath, const Json &json_object);

void print_json(const Json &json_object);

auto json_to_string(const Json &json_object) -> String;

} // namespace pxd