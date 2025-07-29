#include <chesscxx/parse.h>
#include <chesscxx/position.h>

#include <cstdlib>
#include <string_view>

namespace {
void verify(bool check) {
  if (!check) std::abort();
}
auto parsePosition(std::string_view str) -> chesscxx::Position {
  auto parsed_position = chesscxx::parse<chesscxx::Position>(str);
  verify(parsed_position.has_value());
  return parsed_position.value();
}
}  // namespace

auto main() -> int {
  chesscxx::Position const position1 =
      parsePosition("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

  chesscxx::Position const position2 = parsePosition(
      "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 10 10");

  verify(position1 != position2);
  verify(chesscxx::RepetitionEqual{}(position1, position2));

  auto position1_repetition_hash = chesscxx::RepetitionHash{}(position1);
  auto position2_repetition_hash = chesscxx::RepetitionHash{}(position2);

  verify(position1_repetition_hash == position2_repetition_hash);

  // These hashes may collide but are likely different
  verify(std::hash<chesscxx::Position>{}(position1) !=
         std::hash<chesscxx::Position>{}(position2));
}
