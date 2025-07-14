#include <chesskit/game.h>
#include <chesskit/game_result.h>
#include <chesskit/parse.h>
#include <chesskit/san_move.h>
#include <chesskit/uci_move.h>

#include <cassert>
#include <optional>
#include <print>
#include <string_view>

namespace {
chesskit::UciMove parseUciMove(std::string_view sv) {
  auto parsedUciMove = chesskit::parse<chesskit::UciMove>(sv);
  assert(parsedUciMove);
  return parsedUciMove.value();
}

chesskit::SanMove parseSanMove(std::string_view sv) {
  auto parsedSanMove = chesskit::parse<chesskit::SanMove>(sv);
  assert(parsedSanMove);
  return parsedSanMove.value();
}
}  // namespace

int main() {
  chesskit::Game game;
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

  assert(game.result() == chesskit::GameResult::kWhiteWins);

  std::println("{}\n", game);
  std::println("{:pgn}\n", game);
  std::println("{:fen}\n", game);
  std::println("{:ascii}\n", game);
  std::println("{:lists}\n", game);
  std::println("{:rep}\n", game);
}
