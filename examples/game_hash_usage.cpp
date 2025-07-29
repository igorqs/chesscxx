#include <chesscxx/game.h>
#include <chesscxx/parse.h>
#include <chesscxx/position.h>
#include <chesscxx/san_move.h>

#include <cstdlib>
#include <functional>
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

void assertEqual(const auto& lhs, const auto& rhs) {
  verify(lhs == rhs);
  verify(std::hash<chesscxx::Game>{}(lhs) == std::hash<chesscxx::Game>{}(rhs));
}

void assertNotEqual(const auto& lhs, const auto& rhs) {
  verify(lhs != rhs);
  // These hashes may collide but are likely different
  verify(std::hash<chesscxx::Game>{}(lhs) != std::hash<chesscxx::Game>{}(rhs));
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

  verify(uninitialized.move(parseSanMove("e4")));
  assertNotEqual(uninitialized, custom);

  assertNotEqual(uninitialized, standard);

  uninitialized.undoMove();
  assertEqual(uninitialized, standard);
}
