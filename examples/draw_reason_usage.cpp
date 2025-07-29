#include <chesscxx/draw_reason.h>
#include <chesscxx/game.h>
#include <chesscxx/parse.h>

#include <optional>
#include <print>
#include <string_view>

namespace {
void printDrawReason(std::optional<chesscxx::DrawReason> draw_reason) {
  if (!draw_reason.has_value()) return;
  std::println("{}", draw_reason.value());
}

void parseFenAndPrintDrawReason(std::string_view str) {
  auto parsed_game =
      chesscxx::parse<chesscxx::Game>(str, chesscxx::parse_as::Fen{});
  if (!parsed_game.has_value()) return;
  printDrawReason(parsed_game.value().drawReason());
}
}  // namespace

auto main() -> int {
  std::println("{}", chesscxx::DrawReason::kStalemate);
  std::println("{}", chesscxx::DrawReason::kThreefoldRepetition);

  parseFenAndPrintDrawReason("k7/8/8/8/8/8/8/7K w - - 0 1");
  parseFenAndPrintDrawReason(
      "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 100 51");
}
