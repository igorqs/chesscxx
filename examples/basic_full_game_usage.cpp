#include <chesskit/game.h>
#include <chesskit/parse.h>
#include <chesskit/san_move.h>
#include <chesskit/uci_move.h>

#include <cassert>
#include <print>
#include <string_view>

namespace {
auto parseSanMove(std::string_view str) -> chesskit::SanMove {
  auto parsed_san_move = chesskit::parse<chesskit::SanMove>(str);
  assert(parsed_san_move);
  return parsed_san_move.value();
}

auto parseUciMove(std::string_view str) -> chesskit::UciMove {
  auto parsed_uci_move = chesskit::parse<chesskit::UciMove>(str);
  assert(parsed_uci_move);
  return parsed_uci_move.value();
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
