#include "json.hpp"

#include "logger.hpp"

#include "core.h"

#include <filesystem>

#include "rapidjson/encodedstream.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/filewritestream.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/stringbuffer.h"

namespace pxd {

Json load_json(const String &filepath) {
  Json json_object;

  if (!std::filesystem::exists(filepath.c_str())) {
    PXD_LOG_ERROR(fmt::format("{} is not exists", filepath.c_str()).c_str());
    return json_object;
  }

  FILE *file = nullptr;

  fopen_s(&file, filepath.c_str(), "r");

  char readbuffer[65536];
  rapidjson::FileReadStream is(file, readbuffer, sizeof(readbuffer));

  rapidjson::AutoUTFInputStream<unsigned, rapidjson::FileReadStream> eis(is);

  json_object.document.ParseStream<0, rapidjson::AutoUTF<unsigned>>(eis);
  json_object.filepath = filepath.c_str();
  json_object.utf_type = eis.GetType();
  json_object.has_bom = eis.HasBOM();

  fclose(file);

  return json_object;
}

Json load_json(String &&filepath) { return load_json(filepath); }

void write_json(String &&filepath, const Json &json_object) {
  FILE *file = nullptr;

  fopen_s(&file, filepath.c_str(), "w");

  char writebuffer[65536];
  rapidjson::FileWriteStream ostream(file, writebuffer, sizeof(writebuffer));

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

String json_to_string(const Json &json_object) {
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