#include <chesskit/game.h>
#include <chesskit/parse.h>
#include <chesskit/san_move.h>
#include <chesskit/uci_move.h>

#include <cassert>
#include <print>
#include <string_view>

namespace {
auto parseSanMove(std::string_view sv) -> chesskit::SanMove {
  auto parsedSanMove = chesskit::parse<chesskit::SanMove>(sv);
  assert(parsedSanMove);
  return parsedSanMove.value();
}

auto parseUciMove(std::string_view sv) -> chesskit::UciMove {
  auto parsedUciMove = chesskit::parse<chesskit::UciMove>(sv);
  assert(parsedUciMove);
  return parsedUciMove.value();
}
}  // namespace

auto main() -> int {
  chesskit::Game game;

  assert(game.move(parseSanMove("e4")));
  assert(game.move(parseUciMove("g7g5")));
  assert(game.move(parseSanMove("Nc3")));
  assert(game.move(parseSanMove("f5")));
  assert(game.move(parseSanMove("Qh5")));

  std::println("{}", game);
}
