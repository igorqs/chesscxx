#include <chesscxx/game.h>
#include <chesscxx/movegen.h>
#include <chesscxx/parse.h>

#include <cstdlib>
#include <print>
#include <string_view>

namespace {
void verify(const auto& check) {
  if (!static_cast<bool>(check)) std::abort();
}
auto parseFen(std::string_view str) -> chesscxx::Game {
  auto parsed_game =
      chesscxx::parse<chesscxx::Game>(str, chesscxx::parse_as::Fen{});
  verify(parsed_game);

  return parsed_game.value();
}

void printGame(const auto& game) {
  std::println("{:fen}", game);
  std::println("{}", chesscxx::legalUciMoves(game));
  std::println("{}\n", chesscxx::legalSanMoves(game));
}
}  // namespace

auto main() -> int {
  chesscxx::Game const game =
      parseFen("3r1bnr/kPpppppp/2R5/8/8/8/1PPPPPPP/1NBQKBNR w K - 1 9");

  printGame(game);
}
