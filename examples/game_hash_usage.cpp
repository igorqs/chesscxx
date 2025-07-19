#include <chesskit/game.h>
#include <chesskit/parse.h>
#include <chesskit/position.h>
#include <chesskit/san_move.h>

#include <cassert>
#include <functional>
#include <string_view>

namespace {
auto parseSanMove(std::string_view str) -> chesskit::SanMove {
  auto parsed_san_move = chesskit::parse<chesskit::SanMove>(str);
  assert(parsed_san_move);
  return parsed_san_move.value();
}

auto parsePosition(std::string_view str) -> chesskit::Position {
  auto parsed_position = chesskit::parse<chesskit::Position>(str);
  assert(parsed_position);
  return parsed_position.value();
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
}  // namespace

auto main() -> int {
  chesskit::Game uninitialized;

  chesskit::Game const standard = chesskit::Game(parsePosition(
      "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"));
  assertEqual(uninitialized, standard);

  chesskit::Game const custom = chesskit::Game(parsePosition(
      "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq - 0 1"));
  assertNotEqual(uninitialized, custom);

  assert(uninitialized.move(parseSanMove("e4")));
  assertNotEqual(uninitialized, custom);

  assertNotEqual(uninitialized, standard);

  uninitialized.undoMove();
  assertEqual(uninitialized, standard);
}
