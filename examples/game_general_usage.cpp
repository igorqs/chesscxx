#include <chesskit/game.h>
#include <chesskit/parse.h>
#include <chesskit/position.h>
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

chesskit::Position parsePosition(std::string_view sv) {
  auto parsedPosition = chesskit::parse<chesskit::Position>(sv);
  assert(parsedPosition);
  return parsedPosition.value();
}
}  // namespace

int main() {
  chesskit::Game game;
  assert(game.startsFromDefaultPosition());
  assert(game.initialPosition() == game.currentPosition());

  game = chesskit::Game(parsePosition(
      "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"));
  assert(game.startsFromDefaultPosition());
  assert(game.initialPosition() == game.currentPosition());

  game = chesskit::Game(parsePosition(
      "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq - 0 1"));
  assert(!game.startsFromDefaultPosition());
  assert(game.initialPosition() == game.currentPosition());

  assert(game.move(parseSanMove("Nf6")));
  assert(game.initialPosition() != game.currentPosition());

  assert(game.move(parseSanMove("e5")));

  std::println("uci moves: {}", game.uciMoves());

  std::println("san moves: {}", game.sanMoves());

  game.reset();
  assert(game.initialPosition() == game.currentPosition());
  std::println("{}", game);
}
