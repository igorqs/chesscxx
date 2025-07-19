#include <chesskit/color.h>
#include <chesskit/parse.h>
#include <chesskit/piece.h>
#include <chesskit/piece_type.h>

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
  auto parsed_piece = chesskit::parse<chesskit::Piece>(str);
  printErrorOrValue(parsed_piece);
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
