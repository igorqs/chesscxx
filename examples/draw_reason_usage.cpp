#include <chesskit/draw_reason.h>
#include <chesskit/game.h>
#include <chesskit/parse.h>

#include <cassert>
#include <optional>
#include <print>
#include <string_view>

void printDrawReason(std::optional<chesskit::DrawReason> draw_reason) {
  assert(draw_reason);
  std::println("{}", draw_reason.value());
}

void parseFenAndPrintDrawReason(std::string_view sv) {
  auto parsedGame =
      chesskit::parse<chesskit::Game>(sv, chesskit::parse_as::Fen{});
  assert(parsedGame);
  printDrawReason(parsedGame.value().drawReason());
}

int main() {
  std::println("{}", chesskit::DrawReason::kStalemate);
  std::println("{}", chesskit::DrawReason::kThreefoldRepetition);

  parseFenAndPrintDrawReason("k7/8/8/8/8/8/8/7K w - - 0 1");
  parseFenAndPrintDrawReason(
      "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 100 51");
}
