#include <chesscxx/game.h>
#include <chesscxx/parse.h>
#include <chesscxx/position.h>
#include <chesscxx/san_move.h>

#include <cstdlib>
#include <print>
#include <string_view>

namespace {
void verify(const auto& check) {
  if (!static_cast<bool>(check)) std::abort();
}

auto parseSanMove(std::string_view str) -> chesscxx::SanMove {
  auto parsed_san_move = chesscxx::parse<chesscxx::SanMove>(str);
  verify(parsed_san_move);
  return parsed_san_move.value();
}

auto parsePosition(std::string_view str) -> chesscxx::Position {
  auto parsed_position = chesscxx::parse<chesscxx::Position>(str);
  verify(parsed_position);
  return parsed_position.value();
}
}  // namespace

auto main() -> int {
  chesscxx::Game game;
  verify(game.startsFromDefaultPosition());
  verify(game.initialPosition() == game.currentPosition());

  game = chesscxx::Game(parsePosition(
      "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"));
  verify(game.startsFromDefaultPosition());
  verify(game.initialPosition() == game.currentPosition());

  game = chesscxx::Game(parsePosition(
      "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq - 0 1"));
  verify(!game.startsFromDefaultPosition());
  verify(game.initialPosition() == game.currentPosition());

  verify(game.move(parseSanMove("Nf6")));
  verify(game.initialPosition() != game.currentPosition());

  verify(game.move(parseSanMove("e5")));

  std::println("uci moves: {}", game.uciMoves());

  std::println("san moves: {}", game.sanMoves());

  game.reset();
  verify(game.initialPosition() == game.currentPosition());
  std::println("{}", game);
}
