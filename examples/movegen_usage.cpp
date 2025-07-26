#include <chesscxx/game.h>
#include <chesscxx/movegen.h>
#include <chesscxx/parse.h>
#include <chesscxx/san_move.h>

#include <cassert>
#include <print>
#include <string_view>

namespace {
auto parseSanMove(std::string_view str) -> chesscxx::SanMove {
  auto parsed_san_move = chesscxx::parse<chesscxx::SanMove>(str);
  assert(parsed_san_move);
  return parsed_san_move.value();
}

void printGame(const auto& game) {
  std::println("{:fen}", game);
  std::println("{}", chesscxx::legalUciMoves(game));
  std::println("{}\n", chesscxx::legalSanMoves(game));
}
}  // namespace

auto main() -> int {
  chesscxx::Game game;
  printGame(game);

  assert(game.move(parseSanMove("e4")));
  printGame(game);

  assert(game.move(parseSanMove("g5")));
  printGame(game);

  assert(game.move(parseSanMove("Nc3")));
  printGame(game);

  assert(game.move(parseSanMove("f5")));
  printGame(game);

  assert(game.move(parseSanMove("Qh5")));
  printGame(game);
}
