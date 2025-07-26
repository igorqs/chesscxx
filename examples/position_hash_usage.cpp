#include <chesscxx/parse.h>
#include <chesscxx/position.h>

#include <cassert>
#include <string_view>

namespace {
auto parsePosition(std::string_view str) -> chesscxx::Position {
  auto parsed_position = chesscxx::parse<chesscxx::Position>(str);
  assert(parsed_position);
  return parsed_position.value();
}
}  // namespace

auto main() -> int {
  chesscxx::Position const position1 =
      parsePosition("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

  chesscxx::Position const position2 = parsePosition(
      "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 10 10");

  assert(position1 != position2);
  assert(chesscxx::RepetitionEqual{}(position1, position2));

  auto position1_repetition_hash = chesscxx::RepetitionHash{}(position1);
  auto position2_repetition_hash = chesscxx::RepetitionHash{}(position2);

  assert(position1_repetition_hash == position2_repetition_hash);

  // These hashes may collide but are likely different
  assert(std::hash<chesscxx::Position>{}(position1) !=
         std::hash<chesscxx::Position>{}(position2));
}
