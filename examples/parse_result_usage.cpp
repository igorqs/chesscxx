#include <chesscxx/parse.h>
#include <chesscxx/square.h>

#include <iterator>
#include <print>
#include <string_view>

auto main() -> int {
  std::string_view const input = "e4 extra input";

  auto result =
      chesscxx::parseFrom<chesscxx::Square>(input.begin(), input.end());
  if (!result) return 1;

  std::println("{}", result.value().parsed_value);
  std::println("{}", result.value().ptr);
  std::println("{}", std::distance(input.begin(), result.value().ptr) == 2);
}
