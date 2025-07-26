#include <chesscxx/game.h>
#include <chesscxx/game_result.h>
#include <chesscxx/parse.h>

#include <cassert>
#include <optional>
#include <print>
#include <string_view>

namespace {
void printErrorOrValue(auto parsed_value) {
  if (parsed_value) {
    std::println("{}", parsed_value.value());
  } else {
    std::println("{}", parsed_value.error());
  }
}

void parseAndPrint(std::string_view str) {
  auto parsed_game_result = chesscxx::parse<chesscxx::GameResult>(str);
  printErrorOrValue(parsed_game_result);
}

void parseFenAndPrintGameResult(std::string_view str) {
  auto parsed_game =
      chesscxx::parse<chesscxx::Game>(str, chesscxx::parse_as::Fen{});
  assert(parsed_game);

  std::optional<chesscxx::GameResult> game_result =
      parsed_game.value().result();
  assert(game_result);

  std::println("{}", game_result.value());
}
}  // namespace

auto main() -> int {
  chesscxx::GameResult const black_wins = chesscxx::GameResult::kBlackWins;
  chesscxx::GameResult const white_wins = chesscxx::GameResult::kWhiteWins;
  chesscxx::GameResult const draw = chesscxx::GameResult::kDraw;
  std::println(R"("{}" "{}" "{}")", black_wins, white_wins, draw);
  std::println(R"("{:v}" "{:v}" "{:v}")", black_wins, white_wins, draw);
  std::println(R"("{:c}" "{:c}" "{:c}")", black_wins, white_wins, draw);

  parseAndPrint("1-0");
  parseAndPrint("0-1");
  parseAndPrint("1/2-1/2");
  parseAndPrint("1-1");
  parseAndPrint("1/2-0");

  parseFenAndPrintGameResult("k7/8/8/8/8/8/8/7K w - - 0 1");
  parseFenAndPrintGameResult("7k/8/5BKN/8/8/8/8/8 b - - 0 1");
}
