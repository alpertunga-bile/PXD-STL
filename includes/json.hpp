#pragma once

#ifndef RAPIDJSON_SSE2
#define RAPIDJSON_SSE2
#endif

#include "rapidjson/rapidjson.h"

#include "rapidjson/document.h"

#include "string.hpp"

namespace pxd {

struct Json {
  String filepath;
  rapidjson::Document document;
  rapidjson::UTFType utf_type;
  bool has_bom = false;
};

Json load_json(String &&filepath);
Json load_json(const String &filepath);

void write_json(String &&filepath, const Json &json_object);

void print_json(const Json &json_object);

String json_to_string(const Json &json_object);

} // namespace pxd