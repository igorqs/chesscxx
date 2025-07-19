#include <chesskit/parse.h>
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

void parseAsUppercaseAndPrint(std::string_view str) {
  auto parsed_piece_type = chesskit::parse<chesskit::PieceType>(
      str, chesskit::parse_as::Uppercase{});

  auto parsed_promotable_piece_type =
      chesskit::parse<chesskit::PromotablePieceType>(
          str, chesskit::parse_as::Uppercase{});

  printErrorOrValue(parsed_piece_type);
  printErrorOrValue(parsed_promotable_piece_type);
}

void parseAsLowercaseAndPrint(std::string_view str) {
  auto parsed_piece_type = chesskit::parse<chesskit::PieceType>(
      str, chesskit::parse_as::Lowercase{});

  auto parsed_promotable_piece_type =
      chesskit::parse<chesskit::PromotablePieceType>(
          str, chesskit::parse_as::Lowercase{});

  printErrorOrValue(parsed_piece_type);
  printErrorOrValue(parsed_promotable_piece_type);
}
}  // namespace

auto main() -> int {
  chesskit::PieceType piece_type = chesskit::PieceType::kQueen;

  chesskit::PromotablePieceType promotable_piece_type =
      chesskit::PromotablePieceType::kQueen;

  std::println("{} {}", piece_type, promotable_piece_type);
  std::println("{:v} {:v}", piece_type, promotable_piece_type);
  std::println("{:u} {:u}", piece_type, promotable_piece_type);
  std::println("{:l} {:l}\n", piece_type, promotable_piece_type);

  chesskit::PieceType cast = chesskit::toPieceType(promotable_piece_type);
  std::println("{}\n", cast);

  parseAsUppercaseAndPrint("R");
  parseAsUppercaseAndPrint("r");
  parseAsLowercaseAndPrint("R");
  parseAsLowercaseAndPrint("r");
  parseAsUppercaseAndPrint("K");
}
