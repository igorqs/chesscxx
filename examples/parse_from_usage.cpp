#include <chesscxx/game.h>
#include <chesscxx/parse.h>

#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <iterator>
#include <print>
#include <ranges>
#include <string_view>
#include <vector>

auto main() -> int {
  std::ifstream file("data/games.pgn", std::ios::binary);
  if (!file) return 1;

  std::vector<char> buffer{std::istreambuf_iterator<char>{file},
                           std::istreambuf_iterator<char>{}};

  // trim trailing spaces; otherwise, these spaces might be parsed as a valid
  // empty game
  auto rit = std::ranges::find_if_not(
      std::ranges::reverse_view(buffer),
      [](unsigned char character) { return std::isspace(character); });

  std::string_view const str(buffer.begin(), rit.base());

  for (const auto *it = str.begin(); it != str.end();) {
    auto result = chesscxx::parseFrom<chesscxx::Game>(it, str.end());
    if (result) {
      std::println("{}", result->parsed_value);
    } else {
      std::println(stderr, "{}", result.error());
      return 1;
    }
    it = result->ptr;
  }

  return 0;
}
