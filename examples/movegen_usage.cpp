#include <chesskit/game.h>
#include <chesskit/movegen.h>
#include <chesskit/parse.h>
#include <chesskit/san_move.h>

#include <cassert>
#include <print>
#include <string_view>

namespace {
chesskit::SanMove parseSanMove(std::string_view sv) {
  auto parsedSanMove = chesskit::parse<chesskit::SanMove>(sv);
  assert(parsedSanMove);
  return parsedSanMove.value();
}

void printGame(const auto& game) {
  std::println("{:fen}", game);
  std::println("{}", chesskit::legalUciMoves(game));
  std::println("{}\n", chesskit::legalSanMoves(game));
}
}  // namespace

int main() {
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
