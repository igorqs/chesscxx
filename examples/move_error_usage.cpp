#include <chesskit/game.h>
#include <chesskit/move_error.h>
#include <chesskit/parse.h>
#include <chesskit/san_move.h>

#include <cassert>
#include <print>
#include <string_view>

void printGameOrMoveError() {}

auto parseSan(std::string_view sv) {
  auto parsed_move = chesskit::parse<chesskit::SanMove>(sv);
  assert(parsed_move);
  return parsed_move.value();
}

void moveAndPrint(auto& game, const auto& move) {
  auto move_result = game.move(move);
  if (move_result) {
    std::println("{} -> {:fen}", move, game);
  } else {
    std::println("{} -> {}", move, move_result.error());
  }
}

int main() {
  std::println("{}", chesskit::MoveError::kMissingPromotionPiece);
  std::println("{}\n", chesskit::MoveError::kKingOrRookMoved);

  chesskit::Game game;
  std::println("{:fen}\n", game);

  moveAndPrint(game, parseSan("e5"));
  moveAndPrint(game, parseSan("O-O"));
  moveAndPrint(game, parseSan("e4"));
  moveAndPrint(game, parseSan("d5"));
  moveAndPrint(game, parseSan("c4"));
  moveAndPrint(game, parseSan("e6"));
  moveAndPrint(game, parseSan("d5"));
  moveAndPrint(game, parseSan("exd5"));
  moveAndPrint(game, parseSan("d5"));
  moveAndPrint(game, parseSan("Qe2"));
  moveAndPrint(game, parseSan("Nf6"));
  moveAndPrint(game, parseSan("Ne7"));
}
