#include <chesskit/parse.h>
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

void parseAsUppercaseAndPrint(std::string_view sv) {
  auto parsedPieceType =
      chesskit::parse<chesskit::PieceType>(sv, chesskit::parse_as::Uppercase{});

  auto parsedPromotablePieceType =
      chesskit::parse<chesskit::PromotablePieceType>(
          sv, chesskit::parse_as::Uppercase{});

  printErrorOrValue(parsedPieceType);
  printErrorOrValue(parsedPromotablePieceType);
}

void parseAsLowercaseAndPrint(std::string_view sv) {
  auto parsedPieceType =
      chesskit::parse<chesskit::PieceType>(sv, chesskit::parse_as::Lowercase{});

  auto parsedPromotablePieceType =
      chesskit::parse<chesskit::PromotablePieceType>(
          sv, chesskit::parse_as::Lowercase{});

  printErrorOrValue(parsedPieceType);
  printErrorOrValue(parsedPromotablePieceType);
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
