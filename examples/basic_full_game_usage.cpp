#include <chesscxx/game.h>
#include <chesscxx/parse.h>
#include <chesscxx/san_move.h>
#include <chesscxx/uci_move.h>

#include <cassert>
#include <print>
#include <string_view>

namespace {
auto parseSanMove(std::string_view str) -> chesscxx::SanMove {
  auto parsed_san_move = chesscxx::parse<chesscxx::SanMove>(str);
  assert(
parsed_san_move
);
  return parsed_san_move.value();
}

auto parseUciMove(std::string_view str) -> chesscxx::UciMove {
  auto parsed_uci_move = chesscxx::parse<chesscxx::UciMove>(str);
  assert(parsed_uci_move);
  return parsed_uci_move.value();
}
}  // namespace

auto main() -> int {
  chesscxx::Game game;

  assert(game.move(parseSanMove("e4")));
  assert(game.move(parseUciMove("g7g5")));
  assert(game.move(parseSanMove("Nc3")));
  assert(game.move(parseSanMove("f5")));
  assert(game.move(parseSanMove("Qh5")));

  std::println("{}", game);
}
