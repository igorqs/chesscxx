#include <chesskit/file.h>
#include <chesskit/parse.h>
#include <chesskit/rank.h>
#include <chesskit/square.h>

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
  auto parsed_square = chesskit::parse<chesskit::Square>(str);
  printErrorOrValue(parsed_square);
}
}  // namespace

auto main() -> int {
  chesskit::Square square_a8{.file = chesskit::File::kA,
                             .rank = chesskit::Rank::k8};
  std::println(R"("{}" "{}")", square_a8, chesskit::index(square_a8));

  chesskit::Square square_h1{.file = chesskit::File::kH,
                             .rank = chesskit::Rank::k1};
  std::println(R"("{}" "{}")", square_h1, chesskit::index(square_h1));

  parseAndPrint("e4");
  parseAndPrint("ax");
  parseAndPrint("x4");
}
