#include <chesscxx/game.h>
#include <chesscxx/move_error.h>
#include <chesscxx/parse.h>
#include <chesscxx/san_move.h>

#include <cassert>
#include <print>
#include <string_view>

namespace {
auto parseSan(std::string_view str) {
  auto parsed_move = chesscxx::parse<chesscxx::SanMove>(str);
  assert(parsed_move);
  return parsed_move.value();
}

void moveAndPrint(chesscxx::Game& game, const auto& move) {
  auto move_result = game.move(move);
  if (move_result) {
    std::println("{} -> {:fen}", move, game);
  } else {
    std::println("{} -> {}", move, move_result.error());
  }
}
}  // namespace

auto main() -> int {
  std::println("{}", chesscxx::MoveError::kMissingPromotionPiece);
  std::println("{}\n", chesscxx::MoveError::kKingOrRookMoved);

  chesscxx::Game game;
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
