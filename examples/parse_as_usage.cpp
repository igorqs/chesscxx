#include <chesscxx/game.h>
#include <chesscxx/parse.h>
#include <chesscxx/piece_type.h>

#include <print>

namespace {
void printErrorOrValue(auto parsed_value) {
  if (parsed_value) {
    std::println("{}", parsed_value.value());
  } else {
    std::println("{}", parsed_value.error());
  }
}
}  // namespace

auto main() -> int {
  auto parsed_piece_type = chesscxx::parse<chesscxx::PieceType>(
      "r", chesscxx::parse_as::Uppercase{});
  printErrorOrValue(parsed_piece_type);

  parsed_piece_type = chesscxx::parse<chesscxx::PieceType>(
      "r", chesscxx::parse_as::Lowercase{});
  printErrorOrValue(parsed_piece_type);

  auto parsed_game = chesscxx::parse<chesscxx::Game>(
      "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq - 0 1",
      chesscxx::parse_as::Fen{});
  printErrorOrValue(parsed_game);

  parsed_game = chesscxx::parse<chesscxx::Game>(
      "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq - 0 1",
      chesscxx::parse_as::Pgn{});
  printErrorOrValue(parsed_game);
}
