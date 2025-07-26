#include <chesscxx/castling_side.h>
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
  auto parsed_san_move = chesscxx::parse<chesscxx::SanMove>(str);
  printErrorOrValue(parsed_san_move);
}
}  // namespace

auto main() -> int {
  chesscxx::SanMove san_move = chesscxx::SanNormalMove{
      .piece_type = chesscxx::PieceType::kKing,
      .origin = chesscxx::PartialSquare(chesscxx::File::kE, std::nullopt),
      .destination = chesscxx::Square(chesscxx::File::kF, chesscxx::Rank::k8),
  };
  std::println("{}", san_move);

  san_move = chesscxx::SanCastlingMove{
      .side = chesscxx::CastlingSide::kKingside,
  };
  std::println("{}", san_move);

  parseAndPrint("Be7xd8=Q+");
  parseAndPrint("e2e4+");
  parseAndPrint("O-O-O");
  parseAndPrint("ke7xd8=Q+");
  parseAndPrint("0-0");
  parseAndPrint("e4");
}
