#include <chesscxx/castling_rights.h>
#include <chesscxx/parse.h>

#include <print>
#include <string_view>

namespace {
void printErrorOrValue(auto parsed_value) {
  if (parsed_value) {
    std::println("\"{}\"", parsed_value.value());
  } else {
    std::println("{}", parsed_value.error());
  }
}

void parseAndPrint(std::string_view str) {
  auto parsed_castling_rights = chesscxx::parse<chesscxx::CastlingRights>(str);
  printErrorOrValue(parsed_castling_rights);
}
}  // namespace

auto main() -> int {
  parseAndPrint("KQkq");
  parseAndPrint("Kk");
  parseAndPrint("-");
  parseAndPrint("");
  parseAndPrint("qkQK");
  parseAndPrint("x");
}
