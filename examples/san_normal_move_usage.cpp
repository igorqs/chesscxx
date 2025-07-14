#include <chesskit/check_indicator.h>
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
  auto parsedSanMove = chesskit::parse<chesskit::SanNormalMove>(sv);
  printErrorOrValue(parsedSanMove);
}
}  // namespace

int main() {
  chesskit::SanNormalMove normal_move = {
      .pieceType = chesskit::PieceType::kPawn,
      .origin = chesskit::PartialSquare(chesskit::File::kE, chesskit::Rank::k7),
      .isCapture = true,
      .destination = chesskit::Square(chesskit::File::kF, chesskit::Rank::k8),
      .promotion = chesskit::PromotablePieceType::kQueen,
      .check_indicator = chesskit::CheckIndicator::kCheckmate,
  };
  std::println("{}", normal_move);

  normal_move = {
      .pieceType = chesskit::PieceType::kKing,
      .origin = chesskit::PartialSquare(chesskit::File::kE, std::nullopt),
      .destination = chesskit::Square(chesskit::File::kF, chesskit::Rank::k8),
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
