#include <chesskit/parse.h>
#include <chesskit/piece.h>
#include <chesskit/piece_placement.h>
#include <chesskit/piece_placement_error.h>
#include <chesskit/square.h>

#include <cassert>
#include <optional>
#include <print>
#include <string_view>

auto parseSquare(std::string_view sv) {
  auto parsedSquare = chesskit::parse<chesskit::Square>(sv);
  assert(parsedSquare);
  return parsedSquare.value();
}

auto parsePiece(std::string_view sv) {
  auto parsedPiece = chesskit::parse<chesskit::Piece>(sv);
  assert(parsedPiece);
  return parsedPiece.value();
}

void setPiece(auto& array, const auto& square, const auto& piece) {
  array[chesskit::index(square)] = piece;
}

void clearSquare(auto& array, const auto& square) {
  array[chesskit::index(square)] = std::nullopt;
}

void printErrorOrValue(auto created_object) {
  if (created_object) {
    std::println("{}", created_object.value());
  } else {
    std::println("{}", created_object.error());
  }
}

void createPiecePlacementAndPrint(const auto& piece_array) {
  auto piece_placement = chesskit::PiecePlacement::fromPieceArray(piece_array);
  printErrorOrValue(piece_placement);
}

int main() {
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
