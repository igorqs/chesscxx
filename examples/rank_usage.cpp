#include <chesskit/parse.h>
#include <chesskit/rank.h>

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
  auto parsedRank = chesskit::parse<chesskit::Rank>(sv);
  printErrorOrValue(parsedRank);
}

int main() {
  chesskit::Rank rank = chesskit::Rank::k4;
  std::println("{}", rank);

  parseAndPrint("8");
  parseAndPrint("x");
}
