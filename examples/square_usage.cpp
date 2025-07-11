#include <chesskit/file.h>
#include <chesskit/parse.h>
#include <chesskit/rank.h>
#include <chesskit/square.h>

#include <print>
#include <string_view>

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

int main() {
  chesskit::Square a8{chesskit::File::kA, chesskit::Rank::k8};
  std::println("\"{}\" \"{}\"", a8, chesskit::index(a8));

  chesskit::Square h1{chesskit::File::kH, chesskit::Rank::k1};
  std::println("\"{}\" \"{}\"", h1, chesskit::index(h1));

  parseAndPrint("e4");
  parseAndPrint("ax");
  parseAndPrint("x4");
}
