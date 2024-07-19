#pragma once

#include "simdjson.h"
#include "string.hpp"

constexpr size_t TOTAL_JSON_INTIME = 3;

namespace pxd {

struct ParserInfos {
  simdjson::ondemand::parser parser;
  simdjson::padded_string json;
  bool is_occupied = false;
};

static ParserInfos parser_infos[TOTAL_JSON_INTIME];

class Json {
public:
  Json() = default;
  Json(const char *filepath);
  Json(const Json &other) = default;
  Json &operator=(const Json &other) = default;
  Json(Json &&other) = default;
  Json &operator=(Json &&other) = default;
  ~Json();

  decltype(auto) operator[](const char *node_name) {
    return parsed_json[node_name];
  }

  decltype(auto) operator[](int index) {
    return parsed_json.get_array().at(index);
  }

  bool load(const char *filepath);

private:
  int parser_index = -1;
  simdjson::ondemand::document parsed_json;
};
} // namespace pxd