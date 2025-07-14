#include <chesskit/castling_rights.h>
#include <chesskit/parse.h>

#include <print>
#include <string_view>

namespace {
void printErrorOrValue(auto parsedValue) {
  if (parsedValue) {
    std::println("\"{}\"", parsedValue.value());
  } else {
    std::println("{}", parsedValue.error());
  }
}

void parseAndPrint(std::string_view sv) {
  auto parsedCastlingRights = chesskit::parse<chesskit::CastlingRights>(sv);
  printErrorOrValue(parsedCastlingRights);
}
}  // namespace

int main() {
  parseAndPrint("KQkq");
  parseAndPrint("Kk");
  parseAndPrint("-");
  parseAndPrint("");
  parseAndPrint("qkQK");
  parseAndPrint("x");
}
