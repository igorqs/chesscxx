#include <chesskit/castling_side.h>

#include <print>

auto main() -> int {
  chesskit::CastlingSide kingside = chesskit::CastlingSide::kKingside;
  chesskit::CastlingSide queenside = chesskit::CastlingSide::kQueenside;
  std::println("{} {}", kingside, queenside);
}
