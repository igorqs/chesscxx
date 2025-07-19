#include <chesskit/parse.h>
#include <chesskit/position.h>

#include <cassert>
#include <string_view>

namespace {
auto parsePosition(std::string_view str) -> chesskit::Position {
  auto parsed_position = chesskit::parse<chesskit::Position>(str);
  assert(parsed_position);
  return parsed_position.value();
}
}  // namespace

auto main() -> int {
  chesskit::Position const position1 =
      parsePosition("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

  chesskit::Position const position2 = parsePosition(
      "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 10 10");

  assert(position1 != position2);
  assert(chesskit::RepetitionEqual{}(position1, position2));

  auto position1_repetition_hash = chesskit::RepetitionHash{}(position1);
  auto position2_repetition_hash = chesskit::RepetitionHash{}(position2);

  assert(position1_repetition_hash == position2_repetition_hash);

  // These hashes may collide but are likely different
  assert(std::hash<chesskit::Position>{}(position1) !=
         std::hash<chesskit::Position>{}(position2));
}
