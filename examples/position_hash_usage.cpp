#include <chesskit/parse.h>
#include <chesskit/position.h>

#include <cassert>
#include <string_view>

namespace {
auto parsePosition(std::string_view sv) -> chesskit::Position {
  auto parsedPosition = chesskit::parse<chesskit::Position>(sv);
  assert(parsedPosition);
  return parsedPosition.value();
}
}  // namespace

auto main() -> int {
  chesskit::Position const p1 =
      parsePosition("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

  chesskit::Position const p2 = parsePosition(
      "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 10 10");

  assert(p1 != p2);
  assert(chesskit::RepetitionEqual{}(p1, p2));

  auto p1_repetition_hash = chesskit::RepetitionHash{}(p1);
  auto p2_repetition_hash = chesskit::RepetitionHash{}(p2);

  assert(p1_repetition_hash == p2_repetition_hash);

  // These hashes may collide but are likely different
  assert(std::hash<chesskit::Position>{}(p1) !=
         std::hash<chesskit::Position>{}(p2));
}
