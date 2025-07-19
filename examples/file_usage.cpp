#include <chesskit/file.h>
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
  auto parsed_file = chesskit::parse<chesskit::File>(str);
  printErrorOrValue(parsed_file);
}
}  // namespace

auto main() -> int {
  chesskit::File file = chesskit::File::kE;
  std::println("{}", file);

  parseAndPrint("a");
  parseAndPrint("x");
}
