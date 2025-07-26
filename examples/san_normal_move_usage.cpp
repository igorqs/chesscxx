#include <chesscxx/check_indicator.h>
#include <chesscxx/file.h>
#include <chesscxx/parse.h>
#include <chesscxx/partial_square.h>
#include <chesscxx/piece_type.h>
#include <chesscxx/rank.h>
#include <chesscxx/san_move.h>
#include <chesscxx/square.h>

#include <optional>
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
  auto parsed_san_move = chesscxx::parse<chesscxx::SanNormalMove>(str);
  printErrorOrValue(parsed_san_move);
}
}  // namespace

auto main() -> int {
  chesscxx::SanNormalMove normal_move = {
      .piece_type = chesscxx::PieceType::kPawn,
      .origin = chesscxx::PartialSquare(chesscxx::File::kE, chesscxx::Rank::k7),
      .is_capture = true,
      .destination = chesscxx::Square(chesscxx::File::kF, chesscxx::Rank::k8),
      .promotion = chesscxx::PromotablePieceType::kQueen,
      .check_indicator = chesscxx::CheckIndicator::kCheckmate,
  };
  std::println("{}", normal_move);

  normal_move = {
      .piece_type = chesscxx::PieceType::kKing,
      .origin = chesscxx::PartialSquare(chesscxx::File::kE, std::nullopt),
      .destination = chesscxx::Square(chesscxx::File::kF, chesscxx::Rank::k8),
  };
  std::println("{}", normal_move);

  parseAndPrint("Be7xd8=Q+");
  parseAndPrint("be7xd8=Q+");
  parseAndPrint("ke7xd8=Q+");
  parseAndPrint("e7xd8=Q#");
  parseAndPrint("Pe7xd8=Q+");
  parseAndPrint("e7xd8=K");
  parseAndPrint("e7d8");
  parseAndPrint("ed8");
  parseAndPrint("7d8");
  parseAndPrint("d8");
  parseAndPrint("d");
  parseAndPrint("8");
  parseAndPrint("O-O-O");
}
