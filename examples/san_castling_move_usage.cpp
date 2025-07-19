#include <chesskit/castling_side.h>
#include <chesskit/check_indicator.h>
#include <chesskit/parse.h>
#include <chesskit/san_move.h>

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
  auto parsed_san_move = chesskit::parse<chesskit::SanCastlingMove>(str);
  printErrorOrValue(parsed_san_move);
}
}  // namespace

auto main() -> int {
  chesskit::SanCastlingMove castling = {
      .side = chesskit::CastlingSide::kKingside,
  };
  std::println("{}", castling);

  castling = {
      .side = chesskit::CastlingSide::kQueenside,
      .check_indicator = chesskit::CheckIndicator::kCheck,
  };
  std::println("{}", castling);

  parseAndPrint("O-O");
  parseAndPrint("O-O-O");
  parseAndPrint("O-O-O+");
  parseAndPrint("O-O-O#");
  parseAndPrint("0-0");
  parseAndPrint("e4");
}
