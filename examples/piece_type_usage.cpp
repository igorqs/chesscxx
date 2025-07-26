#include <chesscxx/parse.h>
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

void parseAsUppercaseAndPrint(std::string_view str) {
  auto parsed_piece_type = chesscxx::parse<chesscxx::PieceType>(
      str, chesscxx::parse_as::Uppercase{});

  auto parsed_promotable_piece_type =
      chesscxx::parse<chesscxx::PromotablePieceType>(
          str, chesscxx::parse_as::Uppercase{});

  printErrorOrValue(parsed_piece_type);
  printErrorOrValue(parsed_promotable_piece_type);
}

void parseAsLowercaseAndPrint(std::string_view str) {
  auto parsed_piece_type = chesscxx::parse<chesscxx::PieceType>(
      str, chesscxx::parse_as::Lowercase{});

  auto parsed_promotable_piece_type =
      chesscxx::parse<chesscxx::PromotablePieceType>(
          str, chesscxx::parse_as::Lowercase{});

  printErrorOrValue(parsed_piece_type);
  printErrorOrValue(parsed_promotable_piece_type);
}
}  // namespace

auto main() -> int {
  chesscxx::PieceType piece_type = chesscxx::PieceType::kQueen;

  chesscxx::PromotablePieceType promotable_piece_type =
      chesscxx::PromotablePieceType::kQueen;

  std::println("{} {}", piece_type, promotable_piece_type);
  std::println("{:v} {:v}", piece_type, promotable_piece_type);
  std::println("{:u} {:u}", piece_type, promotable_piece_type);
  std::println("{:l} {:l}\n", piece_type, promotable_piece_type);

  chesscxx::PieceType cast = chesscxx::toPieceType(promotable_piece_type);
  std::println("{}\n", cast);

  parseAsUppercaseAndPrint("R");
  parseAsUppercaseAndPrint("r");
  parseAsLowercaseAndPrint("R");
  parseAsLowercaseAndPrint("r");
  parseAsUppercaseAndPrint("K");
}
