#include <chesskit/color.h>
#include <chesskit/parse.h>
#include <chesskit/piece.h>
#include <chesskit/piece_type.h>

#include <print>
#include <string_view>

namespace {
void printErrorOrValue(auto parsedValue) {
  if (parsedValue) {
    std::println("{}", parsedValue.value());
  } else {
    std::println("{}", parsedValue.error());
  }
}

void parseAndPrint(std::string_view sv) {
  auto parsedPiece = chesskit::parse<chesskit::Piece>(sv);
  printErrorOrValue(parsedPiece);
}
}  // namespace

auto main() -> int {
  chesskit::Piece white_queen{.type = chesskit::PieceType::kQueen,
                              .color = chesskit::Color::kWhite};
  std::println("{}", white_queen);
  std::println("{:v}", white_queen);
  std::println("{:c}", white_queen);

  chesskit::Piece black_queen{.type = chesskit::PieceType::kQueen,
                              .color = chesskit::Color::kBlack};
  std::println("{}", black_queen);
  std::println("{:v}", black_queen);
  std::println("{:c}", black_queen);

  parseAndPrint("P");
  parseAndPrint("p");
  parseAndPrint("N");
  parseAndPrint("n");
  parseAndPrint("x");
  parseAndPrint("");
}
