#include <chesskit/castling_side.h>

#include <print>

int main() {
  chesskit::CastlingSide kingside = chesskit::CastlingSide::kKingside;
  chesskit::CastlingSide queenside = chesskit::CastlingSide::kQueenside;
  std::println("{} {}", kingside, queenside);
}
