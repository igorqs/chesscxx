#include <chesscxx/castling_side.h>
#include <chesscxx/check_indicator.h>
#include <chesscxx/parse.h>
#include <chesscxx/san_move.h>

#include <print>
#include <string_view>

namespace {
void printErrorOrValue(auto parsed_value) {
  if (parsed_value) {
    std::println("{}", parsed_value.value());
  } else {
    std::println("{}", parsed_value.error());
  }
}

void parseAndPrint(std::string_view str) {
  auto parsed_san_move = chesscxx::parse<chesscxx::SanCastlingMove>(str);
  printErrorOrValue(parsed_san_move);
}
}  // namespace

auto main() -> int {
  chesscxx::SanCastlingMove castling = {
      .side = chesscxx::CastlingSide::kKingside,
  };
  std::println("{}", castling);

  castling = {
      .side = chesscxx::CastlingSide::kQueenside,
      .check_indicator = chesscxx::CheckIndicator::kCheck,
  };
  std::println("{}", castling);

  parseAndPrint("O-O");
  parseAndPrint("O-O-O");
  parseAndPrint("O-O-O+");
  parseAndPrint("O-O-O#");
  parseAndPrint("0-0");
  parseAndPrint("e4");
}
