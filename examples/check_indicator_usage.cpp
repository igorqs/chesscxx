#include <chesskit/check_indicator.h>
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
  auto parsed_check_indicator = chesskit::parse<chesskit::CheckIndicator>(str);
  printErrorOrValue(parsed_check_indicator);
}
}  // namespace

auto main() -> int {
  chesskit::CheckIndicator check = chesskit::CheckIndicator::kCheck;
  chesskit::CheckIndicator checkmate = chesskit::CheckIndicator::kCheckmate;
  std::println("{} {}", check, checkmate);
  std::println("{:v} {:v}", check, checkmate);
  std::println("{:c} {:c}", check, checkmate);

  parseAndPrint("+");
  parseAndPrint("#");
  parseAndPrint("++");
  parseAndPrint("c");
}
