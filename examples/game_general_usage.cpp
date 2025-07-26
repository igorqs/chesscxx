#include <chesscxx/game.h>
#include <chesscxx/parse.h>
#include <chesscxx/position.h>
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

auto parsePosition(std::string_view str) -> chesscxx::Position {
  auto parsed_position = chesscxx::parse<chesscxx::Position>(str);
  assert(parsed_position);
  return parsed_position.value();
}
}  // namespace

auto main() -> int {
  chesscxx::Game game;
  assert(game.startsFromDefaultPosition());
  assert(game.initialPosition() == game.currentPosition());

  game = chesscxx::Game(parsePosition(
      "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"));
  assert(game.startsFromDefaultPosition());
  assert(game.initialPosition() == game.currentPosition());

  game = chesscxx::Game(parsePosition(
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
