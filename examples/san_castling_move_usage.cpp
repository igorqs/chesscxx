#include <chesskit/castling_side.h>
#include <chesskit/check_indicator.h>
#include <chesskit/parse.h>
#include <chesskit/san_move.h>

#include <print>
#include <string_view>

void printErrorOrValue(auto parsedValue) {
  if (parsedValue) {
    std::println("{}", parsedValue.value());
  } else {
    std::println("{}", parsedValue.error());
  }
}

void parseAndPrint(std::string_view sv) {
  auto parsedSanMove = chesskit::parse<chesskit::SanCastlingMove>(sv);
  printErrorOrValue(parsedSanMove);
}

int main() {
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
