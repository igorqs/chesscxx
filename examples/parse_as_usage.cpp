#include <chesskit/game.h>
#include <chesskit/parse.h>
#include <chesskit/piece_type.h>

#include <print>

void printErrorOrValue(auto parsedValue) {
  if (parsedValue) {
    std::println("{}", parsedValue.value());
  } else {
    std::println("{}", parsedValue.error());
  }
}

int main() {
  auto parsedPieceType = chesskit::parse<chesskit::PieceType>(
      "r", chesskit::parse_as::Uppercase{});
  printErrorOrValue(parsedPieceType);

  parsedPieceType = chesskit::parse<chesskit::PieceType>(
      "r", chesskit::parse_as::Lowercase{});
  printErrorOrValue(parsedPieceType);

  auto parsedGame = chesskit::parse<chesskit::Game>(
      "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq - 0 1",
      chesskit::parse_as::Fen{});
  printErrorOrValue(parsedGame);

  parsedGame = chesskit::parse<chesskit::Game>(
      "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq - 0 1",
      chesskit::parse_as::Pgn{});
  printErrorOrValue(parsedGame);
}
