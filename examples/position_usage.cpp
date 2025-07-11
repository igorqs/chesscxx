#include <chesskit/castling_rights.h>
#include <chesskit/color.h>
#include <chesskit/parse.h>
#include <chesskit/piece_placement.h>
#include <chesskit/position.h>

#include <cassert>
#include <optional>
#include <print>
#include <string_view>

void printErrorOrValue(auto parsedValue) {
  if (parsedValue) {
    std::println("{}", parsedValue.value());
  } else {
    std::println("{}", parsedValue.error());
  }
}

void parseAndPrint(std::string_view sv) {
  auto parsedPosition = chesskit::parse<chesskit::Position>(sv);
  printErrorOrValue(parsedPosition);
}

int main() {
  chesskit::Position position;
  std::println("{}\n", position);
  std::println("{:fen}\n", position);
  std::println("{:ascii}\n", position);
  std::println("{:lists}\n", position);
  std::println("{:rep}\n", position);

  assert(position.piecePlacement() == chesskit::PiecePlacement{});
  assert(position.activeColor() == chesskit::Color::kWhite);
  assert(position.enPassantTargetSquare() == std::nullopt);
  assert(position.halfmoveClock() == 0);
  assert(position.fullmoveNumber() == 1);
  assert(position.castlingRights() == chesskit::CastlingRights{});

  parseAndPrint("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
  parseAndPrint("8/8/8/8/8/8/8/8 w KQkq - 0 1");
  parseAndPrint("xnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
  parseAndPrint("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR x KQkq - 0 1");
  parseAndPrint("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 -1");
  parseAndPrint("k7/1B6/8/8/8/8/8/7K w - - 0 1");
}
