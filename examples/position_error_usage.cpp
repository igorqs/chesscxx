#include <chesskit/castling_rights.h>
#include <chesskit/color.h>
#include <chesskit/parse.h>
#include <chesskit/piece_placement.h>
#include <chesskit/position.h>
#include <chesskit/position_error.h>

#include <cassert>
#include <print>

namespace {
void printErrorOrValue(auto created_object) {
  if (created_object) {
    std::println("{}", created_object.value());
  } else {
    std::println("{}", created_object.error());
  }
}

void createPositionAndPrint(const auto& params) {
  auto position = chesskit::Position::fromParams(params);
  printErrorOrValue(position);
}
}  // namespace

auto main() -> int {
  std::println("{}", chesskit::PositionError::kEnPassantTargetSquareOccupied);
  std::println("{}\n", chesskit::PositionError::kSideNotToMoveIsUnderAttack);

  chesskit::Position::Params params = {.fullmoveNumber = 0};
  createPositionAndPrint(params);

  params.fullmoveNumber = 1;
  params.halfmoveClock = 10;
  createPositionAndPrint(params);

  auto pp = chesskit::parse<chesskit::PiecePlacement>("k7/1B6/8/8/8/8/8/7K");
  assert(pp);
  params.halfmoveClock = 0;
  params.piecePlacement = pp.value();
  createPositionAndPrint(params);

  params.castlingRights = chesskit::CastlingRights(0);
  createPositionAndPrint(params);

  params.activeColor = chesskit::Color::kBlack;
  createPositionAndPrint(params);
}
