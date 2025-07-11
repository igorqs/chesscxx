#include <chesskit/castling_rights.h>
#include <chesskit/castling_side.h>
#include <chesskit/color.h>

#include <bitset>
#include <cassert>
#include <print>

void printCastlingRights(const auto& rights) {
  bool whiteKingside = rights.canCastle(chesskit::CastlingSide::kKingside,
                                        chesskit::Color::kWhite);

  bool whiteQueenside = rights.canCastle(chesskit::CastlingSide::kQueenside,
                                         chesskit::Color::kWhite);

  bool blackKingside = rights.canCastle(chesskit::CastlingSide::kKingside,
                                        chesskit::Color::kBlack);

  bool blackQueenside = rights.canCastle(chesskit::CastlingSide::kQueenside,
                                         chesskit::Color::kBlack);

  std::println("\"{}\"", rights);
  std::println("{} {} {} {} {} {} {}", whiteKingside, whiteQueenside,
               blackKingside, blackQueenside, rights.all(), rights.any(),
               rights.none());
}

int main() {
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
