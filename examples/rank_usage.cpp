#include <chesscxx/parse.h>
#include <chesscxx/rank.h>

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
  auto parsed_rank = chesscxx::parse<chesscxx::Rank>(str);
  printErrorOrValue(parsed_rank);
}
}  // namespace

auto main() -> int {
  chesscxx::Rank rank = chesscxx::Rank::k4;
  std::println("{}", rank);

  parseAndPrint("8");
  parseAndPrint("x");
}
