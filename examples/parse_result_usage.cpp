#include <chesskit/parse.h>
#include <chesskit/square.h>

#include <cassert>
#include <iterator>
#include <print>
#include <string_view>

auto main() -> int {
  std::string_view const sv = "e4 extra input";

  auto result = chesskit::parseFrom<chesskit::Square>(sv.begin(), sv.end());
  assert(result);

  std::println("{}", result.value().parsedValue);
  std::println("{}", result.value().ptr);
  assert(std::distance(sv.begin(), result.value().ptr) == 2);
}
