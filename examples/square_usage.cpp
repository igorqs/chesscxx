#include <chesskit/file.h>
#include <chesskit/parse.h>
#include <chesskit/rank.h>
#include <chesskit/square.h>

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
  auto parsedSquare = chesskit::parse<chesskit::Square>(sv);
  printErrorOrValue(parsedSquare);
}
}  // namespace

auto main() -> int {
  chesskit::Square a8{.file = chesskit::File::kA, .rank = chesskit::Rank::k8};
  std::println(R"("{}" "{}")", a8, chesskit::index(a8));

  chesskit::Square h1{.file = chesskit::File::kH, .rank = chesskit::Rank::k1};
  std::println(R"("{}" "{}")", h1, chesskit::index(h1));

  parseAndPrint("e4");
  parseAndPrint("ax");
  parseAndPrint("x4");
}
