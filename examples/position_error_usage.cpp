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

  chesskit::Position::Params params = {.fullmove_number = 0};
  createPositionAndPrint(params);

  params.fullmove_number = 1;
  createPositionAndPrint(params);

  auto piece_placement =
      chesskit::parse<chesskit::PiecePlacement>("k7/1B6/8/8/8/8/8/7K");
  assert(piece_placement);
  params.halfmove_clock = 0;
  params.piece_placement = piece_placement.value();
  createPositionAndPrint(params);

  params.castling_rights = chesskit::CastlingRights(0);
  createPositionAndPrint(params);

  params.active_color = chesskit::Color::kBlack;
  createPositionAndPrint(params);
}
