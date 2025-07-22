#include <chesskit/parse.h>
#include <chesskit/piece.h>
#include <chesskit/piece_placement.h>
#include <chesskit/piece_placement_error.h>
#include <chesskit/square.h>

#include <cassert>
#include <optional>
#include <print>
#include <string_view>

namespace {
auto parseSquare(std::string_view str) {
  auto parsed_square = chesskit::parse<chesskit::Square>(str);
  assert(parsed_square);
  return parsed_square.value();
}

auto parsePiece(std::string_view str) {
  auto parsed_piece = chesskit::parse<chesskit::Piece>(str);
  assert(parsed_piece);
  return parsed_piece.value();
}

void setPiece(auto& array, const auto& square, const auto& piece) {
  array.at(chesskit::index(square)) = piece;
}

void clearSquare(auto& array, const auto& square) {
  array.at(chesskit::index(square)) = std::nullopt;
}

void printErrorOrValue(const auto& piece_placement) {
  if (piece_placement) {
    std::println("{}", piece_placement.value());
  } else {
    std::println("{}", piece_placement.error());
  }
}

void createPiecePlacementAndPrint(const auto& piece_array) {
  auto piece_placement = chesskit::PiecePlacement::fromPieceArray(piece_array);
  printErrorOrValue(piece_placement);
}
}  // namespace

auto main() -> int {
  std::println("{}", chesskit::PiecePlacementError::kMissingKing);
  std::println("{}\n", chesskit::PiecePlacementError::kPawnOnBackRank);

  chesskit::PiecePlacement::PieceArray array;
  createPiecePlacementAndPrint(array);

  setPiece(array, parseSquare("a8"), parsePiece("K"));  // white king
  setPiece(array, parseSquare("a7"), parsePiece("K"));  // white king
  setPiece(array, parseSquare("h1"), parsePiece("k"));  // black king
  createPiecePlacementAndPrint(array);

  clearSquare(array, parseSquare("a7"));
  createPiecePlacementAndPrint(array);

  setPiece(array, parseSquare("h8"), parsePiece("P"));  // white pawn
  createPiecePlacementAndPrint(array);

  setPiece(array, parseSquare("h8"), parsePiece("p"));  // black pawn
  createPiecePlacementAndPrint(array);
}
