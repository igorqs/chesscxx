#include <chesscxx/game.h>
#include <chesscxx/parse.h>
#include <chesscxx/position.h>
#include <chesscxx/san_move.h>

#include <cassert>
#include <functional>
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

void assertEqual(const auto& lhs, const auto& rhs) {
  assert(lhs == rhs);
  assert(std::hash<chesscxx::Game>{}(lhs) == std::hash<chesscxx::Game>{}(rhs));
}

void assertNotEqual(const auto& lhs, const auto& rhs) {
  assert(lhs != rhs);
  // These hashes may collide but are likely different
  assert(std::hash<chesscxx::Game>{}(lhs) != std::hash<chesscxx::Game>{}(rhs));
}
}  // namespace

auto main() -> int {
  chesscxx::Game uninitialized;

  chesscxx::Game const standard = chesscxx::Game(parsePosition(
      "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"));
  assertEqual(uninitialized, standard);

  chesscxx::Game const custom = chesscxx::Game(parsePosition(
      "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq - 0 1"));
  assertNotEqual(uninitialized, custom);

  assert(uninitialized.move(parseSanMove("e4")));
  assertNotEqual(uninitialized, custom);

  assertNotEqual(uninitialized, standard);

  uninitialized.undoMove();
  assertEqual(uninitialized, standard);
}
