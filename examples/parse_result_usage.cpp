#include <chesskit/parse.h>
#include <chesskit/square.h>

#include <cassert>
#include <iterator>
#include <print>
#include <string_view>

auto main() -> int {
  std::string_view const input = "e4 extra input";

  auto result =
      chesskit::parseFrom<chesskit::Square>(input.begin(), input.end());
  assert(result);

  std::println("{}", result.value().parsed_value);
  std::println("{}", result.value().ptr);
  assert(std::distance(input.begin(), result.value().ptr) == 2);
}
