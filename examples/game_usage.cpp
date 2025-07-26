#include <chesscxx/game.h>
#include <chesscxx/game_result.h>
#include <chesscxx/parse.h>
#include <chesscxx/san_move.h>
#include <chesscxx/uci_move.h>

#include <cassert>
#include <optional>
#include <print>
#include <string_view>

namespace {
auto parseUciMove(std::string_view str) -> chesscxx::UciMove {
  auto parsed_uci_move = chesscxx::parse<chesscxx::UciMove>(str);
  assert(parsed_uci_move);
  return parsed_uci_move.value();
}

auto parseSanMove(std::string_view str) -> chesscxx::SanMove {
  auto parsed_san_move = chesscxx::parse<chesscxx::SanMove>(str);
  assert(parsed_san_move);
  return parsed_san_move.value();
}
}  // namespace

auto main() -> int {
  chesscxx::Game game;
  std::println("{}\n", game);
  std::println("{:pgn}\n", game);
  std::println("{:fen}\n", game);
  std::println("{:ascii}\n", game);
  std::println("{:lists}\n", game);
  std::println("{:rep}\n", game);

  assert(game.move(parseSanMove("xe4+")));

  assert(game.move(parseUciMove("e7e5")));

  assert(game.move(parseSanMove("Qh5")));

  assert(game.move(parseSanMove("Nc6")));

  assert(game.move(parseUciMove("f1c4")));

  assert(game.move(parseSanMove("Nf6")));

  assert(game.move(parseSanMove("Bf7")));
  assert(game.result() == std::nullopt);
  game.undoMove();

  assert(game.move(parseSanMove("Qh5f7")));

  assert(game.result() == chesscxx::GameResult::kWhiteWins);

  std::println("{}\n", game);
  std::println("{:pgn}\n", game);
  std::println("{:fen}\n", game);
  std::println("{:ascii}\n", game);
  std::println("{:lists}\n", game);
  std::println("{:rep}\n", game);
}
