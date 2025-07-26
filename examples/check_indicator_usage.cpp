#include <chesscxx/check_indicator.h>
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
  auto parsed_check_indicator = chesscxx::parse<chesscxx::CheckIndicator>(str);
  printErrorOrValue(parsed_check_indicator);
}
}  // namespace

auto main() -> int {
  chesscxx::CheckIndicator check = chesscxx::CheckIndicator::kCheck;
  chesscxx::CheckIndicator checkmate = chesscxx::CheckIndicator::kCheckmate;
  std::println("{} {}", check, checkmate);
  std::println("{:v} {:v}", check, checkmate);
  std::println("{:c} {:c}", check, checkmate);

  parseAndPrint("+");
  parseAndPrint("#");
  parseAndPrint("++");
  parseAndPrint("c");
}
