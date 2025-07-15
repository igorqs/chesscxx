#include <chesskit/game.h>
#include <chesskit/game_result.h>
#include <chesskit/parse.h>

#include <cassert>
#include <optional>
#include <print>
#include <string_view>

namespace {
void printErrorOrValue(auto parsedValue) {
  if (parsedValue) {
    std::println("{}", parsedValue.value());
  } else {
    std::println("{}", parsedValue.error());
  }
}

void parseAndPrint(std::string_view sv) {
  auto parsedGameResult = chesskit::parse<chesskit::GameResult>(sv);
  printErrorOrValue(parsedGameResult);
}

void parseFenAndPrintGameResult(std::string_view sv) {
  auto parsedGame =
      chesskit::parse<chesskit::Game>(sv, chesskit::parse_as::Fen{});
  assert(parsedGame);

  std::optional<chesskit::GameResult> gameResult = parsedGame.value().result();
  assert(gameResult);

  std::println("{}", gameResult.value());
}
}  // namespace

auto main() -> int {
  chesskit::GameResult blackWins = chesskit::GameResult::kBlackWins;
  chesskit::GameResult whiteWins = chesskit::GameResult::kWhiteWins;
  chesskit::GameResult draw = chesskit::GameResult::kDraw;
  std::println(R"("{}" "{}" "{}")", blackWins, whiteWins, draw);
  std::println(R"("{:v}" "{:v}" "{:v}")", blackWins, whiteWins, draw);
  std::println(R"("{:c}" "{:c}" "{:c}")", blackWins, whiteWins, draw);

  parseAndPrint("1-0");
  parseAndPrint("0-1");
  parseAndPrint("1/2-1/2");
  parseAndPrint("1-1");
  parseAndPrint("1/2-0");

  parseFenAndPrintGameResult("k7/8/8/8/8/8/8/7K w - - 0 1");
  parseFenAndPrintGameResult("7k/8/5BKN/8/8/8/8/8 b - - 0 1");
}
