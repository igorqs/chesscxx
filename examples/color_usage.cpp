#include <chesscxx/color.h>
#include <chesscxx/parse.h>

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
  auto parsed_color = chesscxx::parse<chesscxx::Color>(str);
  printErrorOrValue(parsed_color);
}
}  // namespace

auto main() -> int {
  chesscxx::Color white = chesscxx::Color::kWhite;
  chesscxx::Color black = chesscxx::Color::kBlack;
  std::println("{} {}", white, black);
  std::println("{:v} {:v}", white, black);
  std::println("{:c} {:c}\n", white, black);

  std::println("{}\n", !white);

  parseAndPrint("w");
  parseAndPrint("b");
  parseAndPrint("g");
}
