#include <chesskit/check_indicator.h>
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
  auto parsedCheckIndicator = chesskit::parse<chesskit::CheckIndicator>(sv);
  printErrorOrValue(parsedCheckIndicator);
}
}  // namespace

int main() {
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
