#include "json.hpp"

#include <filesystem>

#include "logger.hpp"

#include "core.h"

namespace pxd {
Json::Json(const char *filepath) { load(filepath); }

Json::~Json() {
  if (parser_index == -1) {
    return;
  }

  parser_infos[parser_index].is_occupied = false;
  parser_index = -1;
}

bool Json::load(const char *filepath) {
  if (!std::filesystem::exists(filepath)) {
    PXD_LOG_WARNING(fmt::format("{} is not exists", filepath).c_str());
    return false;
  }

  for (int i = 0; i < TOTAL_JSON_INTIME; i++) {
    if (!parser_infos[i].is_occupied) {
      parser_index = i;
      break;
    }
  }

  if (parser_index == -1) {
    PXD_LOG_WARNING(
        "All parsers are busy. Increase the TOTAL_JSON_TIME variable's value");
    return false;
  }

  parser_infos[parser_index].is_occupied = true;

  json = simdjson::padded_string::load(filepath);
  parsed_json = parser_infos[parser_index].parser.iterate(json);

  return true;
}
} // namespace pxd