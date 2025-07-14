#include <chesskit/game.h>
#include <chesskit/movegen.h>
#include <chesskit/parse.h>

#include <cassert>
#include <print>
#include <string_view>

namespace {
chesskit::Game parseFen(std::string_view sv) {
  auto parsedGame =
      chesskit::parse<chesskit::Game>(sv, chesskit::parse_as::Fen{});
  assert(parsedGame);

  return parsedGame.value();
}

void printGame(const auto& game) {
  std::println("{:fen}", game);
  std::println("{}", chesskit::legalUciMoves(game));
  std::println("{}\n", chesskit::legalSanMoves(game));
}
}  // namespace

int main() {
  chesskit::Game const game =
      parseFen("3r1bnr/kPpppppp/2R5/8/8/8/1PPPPPPP/1NBQKBNR w K - 1 9");

  printGame(game);
}
