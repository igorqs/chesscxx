#include <chesskit/file.h>
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
  auto parsedFile = chesskit::parse<chesskit::File>(sv);
  printErrorOrValue(parsedFile);
}
}  // namespace

auto main() -> int {
  chesskit::File file = chesskit::File::kE;
  std::println("{}", file);

  parseAndPrint("a");
  parseAndPrint("x");
}
