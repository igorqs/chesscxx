#include <chesscxx/castling_rights.h>
#include <chesscxx/color.h>
#include <chesscxx/parse.h>
#include <chesscxx/piece_placement.h>
#include <chesscxx/position.h>

#include <cstdlib>
#include <optional>
#include <print>
#include <string_view>

namespace {
void verify(bool check) {
  if (!check) std::abort();
}

void printErrorOrValue(auto parsed_value) {
  if (parsed_value) {
    std::println("{}", parsed_value.value());
  } else {
    std::println("{}", parsed_value.error());
  }
}

void parseAndPrint(std::string_view str) {
  auto parsed_position = chesscxx::parse<chesscxx::Position>(str);
  printErrorOrValue(parsed_position);
}
}  // namespace

auto main() -> int {
  chesscxx::Position position;
  std::println("{}\n", position);
  std::println("{:fen}\n", position);
  std::println("{:ascii}\n", position);
  std::println("{:lists}\n", position);
  std::println("{:rep}\n", position);

  verify(position.piecePlacement() == chesscxx::PiecePlacement{});
  verify(position.activeColor() == chesscxx::Color::kWhite);
  verify(position.enPassantTargetSquare() == std::nullopt);
  verify(position.halfmoveClock() == 0);
  verify(position.fullmoveNumber() == 1);
  verify(position.castlingRights() == chesscxx::CastlingRights{});

  parseAndPrint("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
  parseAndPrint("8/8/8/8/8/8/8/8 w KQkq - 0 1");
  parseAndPrint("xnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
  parseAndPrint("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR x KQkq - 0 1");
  parseAndPrint("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 -1");
  parseAndPrint("k7/1B6/8/8/8/8/8/7K w - - 0 1");
}
