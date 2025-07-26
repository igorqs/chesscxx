#include <chesscxx/castling_side.h>

#include <print>

auto main() -> int {
  chesscxx::CastlingSide kingside = chesscxx::CastlingSide::kKingside;
  chesscxx::CastlingSide queenside = chesscxx::CastlingSide::kQueenside;
  std::println("{} {}", kingside, queenside);
}
