#include <chesskit/castling_rights.h>
#include <chesskit/castling_side.h>
#include <chesskit/color.h>

#include <bitset>
#include <cassert>
#include <print>

namespace {
void printCastlingRights(const auto& rights) {
  bool white_kingside = rights.canCastle(chesskit::CastlingSide::kKingside,
                                         chesskit::Color::kWhite);

  bool white_queenside = rights.canCastle(chesskit::CastlingSide::kQueenside,
                                          chesskit::Color::kWhite);

  bool black_kingside = rights.canCastle(chesskit::CastlingSide::kKingside,
                                         chesskit::Color::kBlack);

  bool black_queenside = rights.canCastle(chesskit::CastlingSide::kQueenside,
                                          chesskit::Color::kBlack);

  std::println("\"{}\"", rights);
  std::println("{} {} {} {} {} {} {}", white_kingside, white_queenside,
               black_kingside, black_queenside, rights.all(), rights.any(),
               rights.none());
}
}  // namespace

auto main() -> int {
  chesskit::CastlingRights rights;
  printCastlingRights(rights);

  std::bitset<chesskit::CastlingRights::kNumCastlingRights> bits;
  bits.set(chesskit::CastlingRights::kWhiteKingside);
  bits.set(chesskit::CastlingRights::kBlackKingside);
  bits.set(chesskit::CastlingRights::kBlackQueenside);
  rights = chesskit::CastlingRights(bits);
  assert(rights.toBitset() == bits);
  printCastlingRights(rights);

  rights.enable(chesskit::CastlingSide::kQueenside, chesskit::Color::kWhite);
  printCastlingRights(rights);

  rights.disable(chesskit::CastlingSide::kQueenside, chesskit::Color::kBlack);
  printCastlingRights(rights);

  rights.disable();
  printCastlingRights(rights);
}
