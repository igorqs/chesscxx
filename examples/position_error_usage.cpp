#include <chesscxx/castling_rights.h>
#include <chesscxx/color.h>
#include <chesscxx/parse.h>
#include <chesscxx/piece_placement.h>
#include <chesscxx/position.h>
#include <chesscxx/position_error.h>

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
  auto position = chesscxx::Position::fromParams(params);
  printErrorOrValue(position);
}
}  // namespace

auto main() -> int {
  std::println("{}", chesscxx::PositionError::kEnPassantTargetSquareOccupied);
  std::println("{}\n", chesscxx::PositionError::kSideNotToMoveIsUnderAttack);

  chesscxx::Position::Params params = {.fullmove_number = 0};
  createPositionAndPrint(params);

  params.fullmove_number = 1;
  createPositionAndPrint(params);

  auto piece_placement =
      chesscxx::parse<chesscxx::PiecePlacement>("k7/1B6/8/8/8/8/8/7K");
  assert(piece_placement);
  params.halfmove_clock = 0;
  params.piece_placement = piece_placement.value();
  createPositionAndPrint(params);

  params.castling_rights = chesscxx::CastlingRights(0);
  createPositionAndPrint(params);

  params.active_color = chesscxx::Color::kBlack;
  createPositionAndPrint(params);
}
