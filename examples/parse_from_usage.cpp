#include <chesskit/game.h>
#include <chesskit/parse.h>

#include <algorithm>
#include <cassert>
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
  assert(file);

  std::vector<char> buffer{std::istreambuf_iterator<char>{file},
                           std::istreambuf_iterator<char>{}};

  // trim trailing spaces; otherwise, these spaces might be parsed as a valid
  // empty game
  auto rit = std::ranges::find_if_not(
      std::ranges::reverse_view(buffer),
      [](unsigned char ch) { return std::isspace(ch); });

  std::string_view const sv(buffer.begin(), rit.base());

  for (auto it = sv.begin(); it != sv.end();) {
    auto result = chesskit::parseFrom<chesskit::Game>(it, sv.end());
    assert(result);
    std::println("{}", result->parsedValue);
    it = result->ptr;
  }
}
