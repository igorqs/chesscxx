#include <chesscxx/game.h>
#include <chesscxx/parse.h>
#include <chesscxx/parse_error.h>

#include <print>
#include <string_view>

namespace {
void printErrorOrValue(auto parsed_value) {
  if (parsed_value) {
    std::println("{:fen}", parsed_value.value());
  } else {
    std::println("{}", parsed_value.error());
  }
}

void parseAsPgnAndPrint(std::string_view str) {
  auto parsed_game = chesscxx::parse<chesscxx::Game>(str);
  printErrorOrValue(parsed_game);
}

void parseAsFenAndPrint(std::string_view str) {
  auto parsed_game =
      chesscxx::parse<chesscxx::Game>(str, chesscxx::parse_as::Fen{});
  printErrorOrValue(parsed_game);
}
}  // namespace

auto main() -> int {
  std::println("{}", chesscxx::ParseError::kInvalidRank);
  std::println("{}\n", chesscxx::ParseError::kExpectingEndOfString);

  parseAsFenAndPrint(
      "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq - 0 1");

  parseAsFenAndPrint(
      "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq - 0 1 ");

  parseAsPgnAndPrint(
      "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1");

  parseAsPgnAndPrint(
      "[FEN \"7k/8/6KN/8/7B/8/8/8 w - - 0 1\"]\n"
      "[FEN \"7k/8/6KN/8/7B/8/8/8 w - - 0 1\"]\n"
      "1. Bf6#");

  parseAsPgnAndPrint(
      "[FEN \"7k/8/6KN/8/7B/8/8/8 w - - 0 1\"]\n"
      "1. Bf6#");

  parseAsPgnAndPrint(
      "[]\n"
      "1. b3 e5 2. Bb2 d6 3. e3 Nf6 4. d4 exd4 5. Qxd4 Be7 6. Nf3 O-O 7. Nbd2 "
      "Nc6 8. Qc3 Nd5 9. Qxg7# 1-0");

  parseAsPgnAndPrint(
      "[Result \"1-0\"]\n"
      "1. b3 e5 2. Bb2 d6 3. e3 Nf6 4. d4 exd4 5. Qxd4 Be7 6. Nf3 O-O 7. Nbd2 "
      "Nc6 8. Qc3 Nd5 9. Qxg7# 1-0");
}
