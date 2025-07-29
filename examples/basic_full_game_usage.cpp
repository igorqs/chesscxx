#include <chesscxx/game.h>
#include <chesscxx/parse.h>
#include <chesscxx/san_move.h>
#include <chesscxx/uci_move.h>

#include <cstdlib>
#include <print>
#include <string_view>

namespace {
void verify(const auto& check) {
  if (!static_cast<bool>(check)) std::abort();
}
auto parseSanMove(std::string_view str) -> chesscxx::SanMove {
  auto parsed_san_move = chesscxx::parse<chesscxx::SanMove>(str);
  verify(parsed_san_move);
  return parsed_san_move.value();
}

auto parseUciMove(std::string_view str) -> chesscxx::UciMove {
  auto parsed_uci_move = chesscxx::parse<chesscxx::UciMove>(str);
  verify(parsed_uci_move);
  return parsed_uci_move.value();
}
}  // namespace

auto main() -> int {
  chesscxx::Game game;

  verify(game.move(parseSanMove("e4")));
  verify(game.move(parseUciMove("g7g5")));
  verify(game.move(parseSanMove("Nc3")));
  verify(game.move(parseSanMove("f5")));
  verify(game.move(parseSanMove("Qh5")));

  std::println("{}", game);
}
