#include <chesskit/parse.h>
#include <chesskit/rank.h>

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
  auto parsed_rank = chesskit::parse<chesskit::Rank>(str);
  printErrorOrValue(parsed_rank);
}
}  // namespace

auto main() -> int {
  chesskit::Rank rank = chesskit::Rank::k4;
  std::println("{}", rank);

  parseAndPrint("8");
  parseAndPrint("x");
}
