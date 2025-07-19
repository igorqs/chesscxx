#include <chesskit/game.h>
#include <chesskit/movegen.h>
#include <chesskit/parse.h>
#include <chesskit/san_move.h>

#include <cassert>
#include <print>
#include <string_view>

namespace {
auto parseSanMove(std::string_view str) -> chesskit::SanMove {
  auto parsed_san_move = chesskit::parse<chesskit::SanMove>(str);
  assert(parsed_san_move);
  return parsed_san_move.value();
}

auto parseFen(std::string_view str) -> chesskit::Game {
  auto parsed_game =
      chesskit::parse<chesskit::Game>(str, chesskit::parse_as::Fen{});
  assert(parsed_game);

  return parsed_game.value();
}

void printGame(const auto& game) {
  std::println("{:fen}", game);
  std::println("{}", chesskit::legalUciMoves(game));
  std::println("{}\n", chesskit::legalSanMoves(game));
}
}  // namespace

auto main() -> int {
  chesskit::Game game = parseFen("r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1");
  printGame(game);

  assert(game.move(parseSanMove("O-O-O")));
  printGame(game);

  assert(game.move(parseSanMove("O-O")));
  printGame(game);
}
