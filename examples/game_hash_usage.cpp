#include <chesskit/game.h>
#include <chesskit/parse.h>
#include <chesskit/position.h>
#include <chesskit/san_move.h>

#include <cassert>
#include <functional>
#include <string_view>

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

void assertEqual(const auto& lhs, const auto& rhs) {
  assert(lhs == rhs);
  assert(std::hash<chesskit::Game>{}(lhs) == std::hash<chesskit::Game>{}(rhs));
}

void assertNotEqual(const auto& lhs, const auto& rhs) {
  assert(lhs != rhs);
  // These hashes may collide but are likely different
  assert(std::hash<chesskit::Game>{}(lhs) != std::hash<chesskit::Game>{}(rhs));
}

int main() {
  chesskit::Game uninitialized;

  chesskit::Game standard = chesskit::Game(parsePosition(
      "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"));
  assertEqual(uninitialized, standard);

  chesskit::Game custom = chesskit::Game(parsePosition(
      "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq - 0 1"));
  assertNotEqual(uninitialized, custom);

  uninitialized.move(parseSanMove("e4"));
  assertNotEqual(uninitialized, custom);

  assertNotEqual(uninitialized, standard);

  uninitialized.undoMove();
  assertEqual(uninitialized, standard);
}
