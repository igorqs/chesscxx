#include <chesskit/game.h>
#include <chesskit/parse.h>

#include <print>

auto main() -> int {
  auto parsed_game = chesskit::parse<chesskit::Game>(
      "[FEN \"7k/8/6KN/8/7B/8/8/8 w - - 0 1\"]\n"
      "\n"
      "1. Bf6#");

  if (parsed_game) {
    std::println("{}", parsed_game.value());
  } else {
    std::println("{}", parsed_game.error());
  }
}
