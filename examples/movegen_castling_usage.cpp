#include <chesscxx/game.h>
#include <chesscxx/movegen.h>
#include <chesscxx/parse.h>
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
  chesscxx::Game game = parseFen("r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1");
  printGame(game);

  verify(game.move(parseSanMove("O-O-O")));
  printGame(game);

  verify(game.move(parseSanMove("O-O")));
  printGame(game);
}
