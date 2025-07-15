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
void printErrorOrValue(auto parsedValue) {
  if (parsedValue) {
    std::println("{}", parsedValue.value());
  } else {
    std::println("{}", parsedValue.error());
  }
}

void parseAndPrint(std::string_view sv) {
  auto parsedSanMove = chesskit::parse<chesskit::SanMove>(sv);
  printErrorOrValue(parsedSanMove);
}
}  // namespace

auto main() -> int {
  chesskit::SanMove san_move = chesskit::SanNormalMove{
      .pieceType = chesskit::PieceType::kKing,
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
