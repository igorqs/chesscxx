#include <chesscxx/color.h>
#include <chesscxx/parse.h>
#include <chesscxx/piece.h>
#include <chesscxx/piece_type.h>

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
  auto parsed_piece = chesscxx::parse<chesscxx::Piece>(str);
  printErrorOrValue(parsed_piece);
}
}  // namespace

auto main() -> int {
  chesscxx::Piece white_queen{.type = chesscxx::PieceType::kQueen,
                              .color = chesscxx::Color::kWhite};
  std::println("{}", white_queen);
  std::println("{:v}", white_queen);
  std::println("{:c}", white_queen);

  chesscxx::Piece black_queen{.type = chesscxx::PieceType::kQueen,
                              .color = chesscxx::Color::kBlack};
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
