#pragma once

#include "simdjson.h"

constexpr size_t TOTAL_JSON_INTIME = 3;

namespace pxd {

struct ParserInfos {
  simdjson::ondemand::parser parser;
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

  bool load(const char *filepath);

private:
  int parser_index = -1;
  simdjson::padded_string json;
  simdjson::ondemand::document parsed_json;
};
} // namespace pxd