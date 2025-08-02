#include <chesscxx/file.h>
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
  auto parsed_file = chesscxx::parse<chesscxx::File>(str);

  printErrorOrValue(parsed_file);
}
}  // namespace

auto main() -> int {
  chesscxx::File file = chesscxx::File::kE;
  std::println("{}", file);

  parseAndPrint("a");
  parseAndPrint("x");
}
