#include <chesskit/game.h>
#include <chesskit/movegen.h>
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

void printGame(const auto& game) {
  std::println("{:fen}", game);
  std::println("{}", chesskit::legalUciMoves(game));
  std::println("{}\n", chesskit::legalSanMoves(game));
}
}  // namespace

auto main() -> int {
  chesskit::Game game;
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
