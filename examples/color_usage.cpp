#include <chesskit/color.h>
#include <chesskit/parse.h>

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
  auto parsedColor = chesskit::parse<chesskit::Color>(sv);
  printErrorOrValue(parsedColor);
}
}  // namespace

int main() {
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
