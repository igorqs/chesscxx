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

void printGame(const auto& game) {
  std::println("{:fen}", game);
  std::println("{}", chesscxx::legalUciMoves(game));
  std::println("{}\n", chesscxx::legalSanMoves(game));
}
}  // namespace

auto main() -> int {
  chesscxx::Game game;
  printGame(game);

  verify(game.move(parseSanMove("e4")));
  printGame(game);

  verify(game.move(parseSanMove("g5")));
  printGame(game);

  verify(game.move(parseSanMove("Nc3")));
  printGame(game);

  verify(game.move(parseSanMove("f5")));
  printGame(game);

  verify(game.move(parseSanMove("Qh5")));
  printGame(game);
}
