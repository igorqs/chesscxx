#include <chesskit/color.h>
#include <chesskit/parse.h>

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
  auto parsed_color = chesskit::parse<chesskit::Color>(str);
  printErrorOrValue(parsed_color);
}
}  // namespace

auto main() -> int {
  chesskit::Color white = chesskit::Color::kWhite;
  chesskit::Color black = chesskit::Color::kBlack;
  std::println("{} {}", white, black);
  std::println("{:v} {:v}", white, black);
  std::println("{:c} {:c}\n", white, black);

  std::println("{}\n", !white);

  parseAndPrint("w");
  parseAndPrint("b");
  parseAndPrint("g");
}
