#include <chesskit/draw_reason.h>
#include <chesskit/game.h>
#include <chesskit/game_result.h>
#include <chesskit/parse.h>
#include <chesskit/san_move.h>

#include <cassert>
#include <print>
#include <string_view>

namespace {
auto parseSanMove(std::string_view str) -> chesskit::SanMove {
  auto parsed_san_move = chesskit::parse<chesskit::SanMove>(str);
  assert(parsed_san_move);
  return parsed_san_move.value();
}

void printRepetition(auto& game) { std::println("{:rep}\n", game); }

void move(auto& game, std::string_view str) {
  auto move_result = game.move(parseSanMove(str));
  assert(move_result);
}
}  // namespace

auto main() -> int {
  chesskit::Game game;
  printRepetition(game);

  move(game, "Nc3");
  printRepetition(game);

  move(game, "Nc6");
  printRepetition(game);

  move(game, "Nb1");
  printRepetition(game);

  move(game, "Nb8");
  printRepetition(game);

  move(game, "Nc3");
  printRepetition(game);

  move(game, "Nc6");
  printRepetition(game);

  assert(game.repetitionTracker().at(game.currentPosition()) == 2);

  move(game, "Nb1");
  printRepetition(game);

  move(game, "Nb8");
  printRepetition(game);

  assert(game.result() == chesskit::GameResult::kDraw);
  assert(game.drawReason() == chesskit::DrawReason::kThreefoldRepetition);
}
