#include <chesskit/file.h>
#include <chesskit/parse.h>
#include <chesskit/position.h>
#include <chesskit/rank.h>
#include <chesskit/square.h>

#include <cassert>
#include <optional>

int main() {
  constexpr auto kE3 = chesskit::Square(chesskit::File::kE, chesskit::Rank::k3);

  auto parsed_position = chesskit::parse<chesskit::Position>(
      "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1");

  assert(parsed_position.has_value());
  assert(parsed_position->enPassantTargetSquare() == kE3);
  assert(parsed_position->legalEnPassantTargetSquare() == std::nullopt);

  parsed_position = chesskit::parse<chesskit::Position>(
      "rnbqkbnr/ppp1pppp/8/8/3pP3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1");

  assert(parsed_position.has_value());
  assert(parsed_position->enPassantTargetSquare() == kE3);
  assert(parsed_position->legalEnPassantTargetSquare() == kE3);

  parsed_position = chesskit::parse<chesskit::Position>(
      "rnbkqbnr/ppp1pppp/8/8/3pP3/3Q4/PPPP1PPP/RNB1KBNR b KQ e3 0 1");

  assert(parsed_position.has_value());
  assert(parsed_position->enPassantTargetSquare() == kE3);
  assert(parsed_position->legalEnPassantTargetSquare() == std::nullopt);
}
