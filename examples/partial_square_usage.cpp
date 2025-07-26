#include <chesscxx/file.h>
#include <chesscxx/parse.h>
#include <chesscxx/partial_square.h>
#include <chesscxx/rank.h>

#include <optional>
#include <print>
#include <string_view>

namespace {
void printErrorOrValue(auto parsed_value) {
  if (parsed_value) {
    std::println("\"{}\"", parsed_value.value());
  } else {
    std::println("{}", parsed_value.error());
  }
}

void parseAndPrint(std::string_view str) {
  auto parsed_partial_square = chesscxx::parse<chesscxx::PartialSquare>(str);
  printErrorOrValue(parsed_partial_square);
}
}  // namespace

auto main() -> int {
  chesscxx::PartialSquare both{.file = chesscxx::File::kA,
                               .rank = chesscxx::Rank::k8};
  std::println("\"{}\"", both);

  chesscxx::PartialSquare file_only{.file = chesscxx::File::kA,
                                    .rank = std::nullopt};
  std::println("\"{}\"", file_only);

  chesscxx::PartialSquare rank_only{.file = std::nullopt,
                                    .rank = chesscxx::Rank::k8};
  std::println("\"{}\"", rank_only);

  chesscxx::PartialSquare neither{.file = std::nullopt, .rank = std::nullopt};
  std::println("\"{}\"", neither);

  parseAndPrint("e4");
  parseAndPrint("e");
  parseAndPrint("4");
  parseAndPrint("");
  parseAndPrint("ax");
  parseAndPrint("x4");
}
