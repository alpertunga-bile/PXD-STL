#include "json.hpp"

#include "filesystem.hpp"
#include "logger.hpp"

#include "core.h"

#include "rapidjson/encodedstream.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/filewritestream.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/stringbuffer.h"

#include "hash.hpp"

namespace pxd {

constexpr int MAX_FILE_BUFFER_SIZE = 65536;

auto load_json(String filepath) -> Json {
  Json json_object;

  if (!pxd::fs::exists(filepath.c_str())) {
    PXD_LOG_ERROR("{} is not exists", filepath.c_str());
    return json_object;
  }

  FILE *file = nullptr;

  fopen_s(&file, filepath.c_str(), "r");

  std::array<char, MAX_FILE_BUFFER_SIZE> readbuffer;
  rapidjson::FileReadStream is(file, readbuffer.data(), MAX_FILE_BUFFER_SIZE);

  rapidjson::AutoUTFInputStream<unsigned, rapidjson::FileReadStream> eis(is);

  json_object.document.ParseStream<0, rapidjson::AutoUTF<unsigned>>(eis);
  json_object.filepath = filepath.c_str();
  json_object.utf_type = eis.GetType();

  pxd::comp_hash(readbuffer.data(), MAX_FILE_BUFFER_SIZE,
                 json_object.content_hash.data());

  fclose(file);

  return json_object;
}

void write_json(String filepath, const Json &json_object) {
  FILE *file = nullptr;

  fopen_s(&file, filepath.c_str(), "w");

  std::array<char, MAX_FILE_BUFFER_SIZE> writebuffer;
  rapidjson::FileWriteStream ostream(file, writebuffer.data(),
                                     MAX_FILE_BUFFER_SIZE);

  rapidjson::AutoUTFOutputStream<unsigned, rapidjson::FileWriteStream> eos(
      ostream, json_object.utf_type, true);

  rapidjson::PrettyWriter<
      rapidjson::AutoUTFOutputStream<unsigned, rapidjson::FileWriteStream>,
      rapidjson::UTF8<>, rapidjson::AutoUTF<unsigned>>
      writer(eos);

  json_object.document.Accept(writer);

  fclose(file);
}

void print_json(const Json &json_object) {
  rapidjson::StringBuffer buffer;

  rapidjson::AutoUTFOutputStream<unsigned, rapidjson::StringBuffer> eos(
      buffer, json_object.utf_type, false);

  rapidjson::PrettyWriter<
      rapidjson::AutoUTFOutputStream<unsigned, rapidjson::StringBuffer>,
      rapidjson::UTF8<>, rapidjson::AutoUTF<unsigned>>
      writer(eos);

  json_object.document.Accept(writer);

  fmt::println("{}", buffer.GetString());
}

auto json_to_string(const Json &json_object) -> String {
  rapidjson::StringBuffer buffer;

  rapidjson::AutoUTFOutputStream<unsigned, rapidjson::StringBuffer> eos(
      buffer, json_object.utf_type, false);

  rapidjson::PrettyWriter<
      rapidjson::AutoUTFOutputStream<unsigned, rapidjson::StringBuffer>,
      rapidjson::UTF8<>, rapidjson::AutoUTF<unsigned>>
      writer(eos);

  json_object.document.Accept(writer);

  return String(buffer.GetString());
}
} // namespace pxd