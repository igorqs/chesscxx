#include <chesskit/file.h>
#include <chesskit/parse.h>
#include <chesskit/partial_square.h>
#include <chesskit/rank.h>

#include <optional>
#include <print>
#include <string_view>

namespace {
void printErrorOrValue(auto parsedValue) {
  if (parsedValue) {
    std::println("\"{}\"", parsedValue.value());
  } else {
    std::println("{}", parsedValue.error());
  }
}

void parseAndPrint(std::string_view sv) {
  auto parsedPartialSquare = chesskit::parse<chesskit::PartialSquare>(sv);
  printErrorOrValue(parsedPartialSquare);
}
}  // namespace

int main() {
  chesskit::PartialSquare both{chesskit::File::kA, chesskit::Rank::k8};
  std::println("\"{}\"", both);

  chesskit::PartialSquare file_only{chesskit::File::kA, std::nullopt};
  std::println("\"{}\"", file_only);

  chesskit::PartialSquare rank_only{std::nullopt, chesskit::Rank::k8};
  std::println("\"{}\"", rank_only);

  chesskit::PartialSquare neither{std::nullopt, std::nullopt};
  std::println("\"{}\"", neither);

  parseAndPrint("e4");
  parseAndPrint("e");
  parseAndPrint("4");
  parseAndPrint("");
  parseAndPrint("ax");
  parseAndPrint("x4");
}
