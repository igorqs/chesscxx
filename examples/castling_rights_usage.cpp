#include <chesscxx/castling_rights.h>
#include <chesscxx/castling_side.h>
#include <chesscxx/color.h>

#include <bitset>
#include <cassert>
#include <print>

namespace {
void printCastlingRights(const auto& rights) {
  bool white_kingside = rights.canCastle(chesscxx::CastlingSide::kKingside,
                                         chesscxx::Color::kWhite);

  bool white_queenside = rights.canCastle(chesscxx::CastlingSide::kQueenside,
                                          chesscxx::Color::kWhite);

  bool black_kingside = rights.canCastle(chesscxx::CastlingSide::kKingside,
                                         chesscxx::Color::kBlack);

  bool black_queenside = rights.canCastle(chesscxx::CastlingSide::kQueenside,
                                          chesscxx::Color::kBlack);

  std::println("\"{}\"", rights);
  std::println("{} {} {} {} {} {} {}", white_kingside, white_queenside,
               black_kingside, black_queenside, rights.all(), rights.any(),
               rights.none());
}
}  // namespace

auto main() -> int {
  chesscxx::CastlingRights rights;
  printCastlingRights(rights);

  std::bitset<chesscxx::CastlingRights::kNumCastlingRights> bits;
  bits.set(chesscxx::CastlingRights::kWhiteKingside);
  bits.set(chesscxx::CastlingRights::kBlackKingside);
  bits.set(chesscxx::CastlingRights::kBlackQueenside);
  rights = chesscxx::CastlingRights(bits);
  assert(rights.toBitset() == bits);
  printCastlingRights(rights);

  rights.enable(chesscxx::CastlingSide::kQueenside, chesscxx::Color::kWhite);
  printCastlingRights(rights);

  rights.disable(chesscxx::CastlingSide::kQueenside, chesscxx::Color::kBlack);
  printCastlingRights(rights);

  rights.disable();
  printCastlingRights(rights);
}
