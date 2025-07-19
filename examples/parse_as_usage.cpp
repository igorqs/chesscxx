#include <chesskit/game.h>
#include <chesskit/parse.h>
#include <chesskit/piece_type.h>

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
  auto parsed_piece_type = chesskit::parse<chesskit::PieceType>(
      "r", chesskit::parse_as::Uppercase{});
  printErrorOrValue(parsed_piece_type);

  parsed_piece_type = chesskit::parse<chesskit::PieceType>(
      "r", chesskit::parse_as::Lowercase{});
  printErrorOrValue(parsed_piece_type);

  auto parsed_game = chesskit::parse<chesskit::Game>(
      "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq - 0 1",
      chesskit::parse_as::Fen{});
  printErrorOrValue(parsed_game);

  parsed_game = chesskit::parse<chesskit::Game>(
      "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq - 0 1",
      chesskit::parse_as::Pgn{});
  printErrorOrValue(parsed_game);
}
