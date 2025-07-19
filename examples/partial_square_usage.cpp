#include <chesskit/file.h>
#include <chesskit/parse.h>
#include <chesskit/partial_square.h>
#include <chesskit/rank.h>

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
  auto parsed_partial_square = chesskit::parse<chesskit::PartialSquare>(str);
  printErrorOrValue(parsed_partial_square);
}
}  // namespace

auto main() -> int {
  chesskit::PartialSquare both{.file = chesskit::File::kA,
                               .rank = chesskit::Rank::k8};
  std::println("\"{}\"", both);

  chesskit::PartialSquare file_only{.file = chesskit::File::kA,
                                    .rank = std::nullopt};
  std::println("\"{}\"", file_only);

  chesskit::PartialSquare rank_only{.file = std::nullopt,
                                    .rank = chesskit::Rank::k8};
  std::println("\"{}\"", rank_only);

  chesskit::PartialSquare neither{.file = std::nullopt, .rank = std::nullopt};
  std::println("\"{}\"", neither);

  parseAndPrint("e4");
  parseAndPrint("e");
  parseAndPrint("4");
  parseAndPrint("");
  parseAndPrint("ax");
  parseAndPrint("x4");
}
