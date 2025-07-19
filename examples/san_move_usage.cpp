#include <chesskit/castling_side.h>
#include <chesskit/file.h>
#include <chesskit/parse.h>
#include <chesskit/partial_square.h>
#include <chesskit/piece_type.h>
#include <chesskit/rank.h>
#include <chesskit/san_move.h>
#include <chesskit/square.h>

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
  auto parsed_san_move = chesskit::parse<chesskit::SanMove>(str);
  printErrorOrValue(parsed_san_move);
}
}  // namespace

auto main() -> int {
  chesskit::SanMove san_move = chesskit::SanNormalMove{
      .piece_type = chesskit::PieceType::kKing,
      .origin = chesskit::PartialSquare(chesskit::File::kE, std::nullopt),
      .destination = chesskit::Square(chesskit::File::kF, chesskit::Rank::k8),
  };
  std::println("{}", san_move);

  san_move = chesskit::SanCastlingMove{
      .side = chesskit::CastlingSide::kKingside,
  };
  std::println("{}", san_move);

  parseAndPrint("Be7xd8=Q+");
  parseAndPrint("e2e4+");
  parseAndPrint("O-O-O");
  parseAndPrint("ke7xd8=Q+");
  parseAndPrint("0-0");
  parseAndPrint("e4");
}
