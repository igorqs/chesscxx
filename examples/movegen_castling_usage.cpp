#include <chesskit/game.h>
#include <chesskit/movegen.h>
#include <chesskit/parse.h>
#include <chesskit/san_move.h>

#include <cassert>
#include <print>
#include <string_view>

chesskit::SanMove parseSanMove(std::string_view sv) {
  auto parsedSanMove = chesskit::parse<chesskit::SanMove>(sv);
  assert(parsedSanMove);
  return parsedSanMove.value();
}

chesskit::Game parseFen(std::string_view sv) {
  auto parsedGame =
      chesskit::parse<chesskit::Game>(sv, chesskit::parse_as::Fen{});
  assert(parsedGame);

  return parsedGame.value();
}

void printGame(const auto& game) {
  std::println("{:fen}", game);
  std::println("{}", chesskit::legalUciMoves(game));
  std::println("{}\n", chesskit::legalSanMoves(game));
}

int main() {
  chesskit::Game game = parseFen("r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1");
  printGame(game);

  assert(game.move(parseSanMove("O-O-O")));
  printGame(game);

  assert(game.move(parseSanMove("O-O")));
  printGame(game);
}
