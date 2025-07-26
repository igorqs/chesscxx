#include <chesscxx/file.h>
#include <chesscxx/parse.h>
#include <chesscxx/rank.h>
#include <chesscxx/square.h>

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
  auto parsed_square = chesscxx::parse<chesscxx::Square>(str);
  printErrorOrValue(parsed_square);
}
}  // namespace

auto main() -> int {
  chesscxx::Square square_a8{.file = chesscxx::File::kA,
                             .rank = chesscxx::Rank::k8};
  std::println(R"("{}" "{}")", square_a8, chesscxx::index(square_a8));

  chesscxx::Square square_h1{.file = chesscxx::File::kH,
                             .rank = chesscxx::Rank::k1};
  std::println(R"("{}" "{}")", square_h1, chesscxx::index(square_h1));

  parseAndPrint("e4");
  parseAndPrint("ax");
  parseAndPrint("x4");
}
