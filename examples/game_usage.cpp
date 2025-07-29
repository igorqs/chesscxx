#include <chesscxx/game.h>
#include <chesscxx/game_result.h>
#include <chesscxx/parse.h>
#include <chesscxx/san_move.h>
#include <chesscxx/uci_move.h>

#include <cstdlib>
#include <optional>
#include <print>
#include <string_view>

namespace {
void verify(const auto& check) {
  if (!static_cast<bool>(check)) std::abort();
}

auto parseUciMove(std::string_view str) -> chesscxx::UciMove {
  auto parsed_uci_move = chesscxx::parse<chesscxx::UciMove>(str);
  verify(parsed_uci_move.has_value());
  return parsed_uci_move.value();
}

auto parseSanMove(std::string_view str) -> chesscxx::SanMove {
  auto parsed_san_move = chesscxx::parse<chesscxx::SanMove>(str);
  verify(parsed_san_move.has_value());
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

  verify(game.move(parseSanMove("xe4+")));

  verify(game.move(parseUciMove("e7e5")));

  verify(game.move(parseSanMove("Qh5")));

  verify(game.move(parseSanMove("Nc6")));

  verify(game.move(parseUciMove("f1c4")));

  verify(game.move(parseSanMove("Nf6")));

  verify(game.move(parseSanMove("Bf7")));
  verify(game.result() == std::nullopt);
  game.undoMove();

  verify(game.move(parseSanMove("Qh5f7")));

  verify(game.result() == chesscxx::GameResult::kWhiteWins);

  std::println("{}\n", game);
  std::println("{:pgn}\n", game);
  std::println("{:fen}\n", game);
  std::println("{:ascii}\n", game);
  std::println("{:lists}\n", game);
  std::println("{:rep}\n", game);
}
