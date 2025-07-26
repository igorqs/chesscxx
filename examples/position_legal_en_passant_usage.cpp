#include <chesscxx/file.h>
#include <chesscxx/parse.h>
#include <chesscxx/position.h>
#include <chesscxx/rank.h>
#include <chesscxx/square.h>

#include <cassert>
#include <optional>

auto main() -> int {
  constexpr auto kE3 = chesscxx::Square(chesscxx::File::kE, chesscxx::Rank::k3);

  auto parsed_position = chesscxx::parse<chesscxx::Position>(
      "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1");

  assert(parsed_position.has_value());
  assert(parsed_position->enPassantTargetSquare() == kE3);
  assert(parsed_position->legalEnPassantTargetSquare() == std::nullopt);

  parsed_position = chesscxx::parse<chesscxx::Position>(
      "rnbqkbnr/ppp1pppp/8/8/3pP3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1");

  assert(parsed_position.has_value());
  assert(parsed_position->enPassantTargetSquare() == kE3);
  assert(parsed_position->legalEnPassantTargetSquare() == kE3);

  parsed_position = chesscxx::parse<chesscxx::Position>(
      "rnbkqbnr/ppp1pppp/8/8/3pP3/3Q4/PPPP1PPP/RNB1KBNR b KQ e3 0 1");

  assert(parsed_position.has_value());
  assert(parsed_position->enPassantTargetSquare() == kE3);
  assert(parsed_position->legalEnPassantTargetSquare() == std::nullopt);
}
