#include <chesscxx/draw_reason.h>
#include <chesscxx/game.h>
#include <chesscxx/game_result.h>
#include <chesscxx/parse.h>
#include <chesscxx/san_move.h>

#include <cstdlib>
#include <print>
#include <string_view>

namespace {
void verify(const auto& check) {
  if (!static_cast<bool>(check)) std::abort();
}
auto parseSanMove(std::string_view str) -> chesscxx::SanMove {
  auto parsed_san_move = chesscxx::parse<chesscxx::SanMove>(str);
  verify(parsed_san_move);
  return parsed_san_move.value();
}

void printRepetition(auto& game) { std::println("{:rep}\n", game); }

void move(auto& game, std::string_view str) {
  auto move_result = game.move(parseSanMove(str));
  verify(move_result);
}
}  // namespace

auto main() -> int {
  chesscxx::Game game;
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

  verify(game.repetitionTracker().at(game.currentPosition()) == 2);

  move(game, "Nb1");
  printRepetition(game);

  move(game, "Nb8");
  printRepetition(game);

  verify(game.result() == chesscxx::GameResult::kDraw);
  verify(game.drawReason() == chesscxx::DrawReason::kThreefoldRepetition);
}
