#pragma once

#ifndef RAPIDJSON_SSE2
#define RAPIDJSON_SSE2
#endif

#include "string.hpp"

#include "rapidjson/rapidjson.h"

#include "rapidjson/document.h"

namespace pxd {

struct Json {
  String filepath;
  uint8_t content_hash[32];
  rapidjson::Document document;
  rapidjson::UTFType utf_type;
};

Json load_json(String &&filepath);
Json load_json(const String &filepath);

void write_json(const String &filepath, const Json &json_object);
void write_json(String &&filepath, const Json &json_object);

void print_json(const Json &json_object);

String json_to_string(const Json &json_object);

} // namespace pxd