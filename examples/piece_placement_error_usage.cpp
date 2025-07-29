#include <chesscxx/parse.h>
#include <chesscxx/piece.h>
#include <chesscxx/piece_placement.h>
#include <chesscxx/piece_placement_error.h>
#include <chesscxx/square.h>

#include <cstdlib>
#include <optional>
#include <print>
#include <string_view>

namespace {
auto parseSquare(std::string_view str) {
  auto parsed_square = chesscxx::parse<chesscxx::Square>(str);
  if (!parsed_square) std::abort();
  return parsed_square.value();
}

auto parsePiece(std::string_view str) {
  auto parsed_piece = chesscxx::parse<chesscxx::Piece>(str);
  if (!parsed_piece) std::abort();
  return parsed_piece.value();
}

void setPiece(auto& array, const auto& square, const auto& piece) {
  array.at(chesscxx::index(square)) = piece;
}

void clearSquare(auto& array, const auto& square) {
  array.at(chesscxx::index(square)) = std::nullopt;
}

void printErrorOrValue(const auto& piece_placement) {
  if (piece_placement) {
    std::println("{}", piece_placement.value());
  } else {
    std::println("{}", piece_placement.error());
  }
}

void createPiecePlacementAndPrint(const auto& piece_array) {
  auto piece_placement = chesscxx::PiecePlacement::fromPieceArray(piece_array);
  printErrorOrValue(piece_placement);
}
}  // namespace

auto main() -> int {
  std::println("{}", chesscxx::PiecePlacementError::kMissingKing);
  std::println("{}\n", chesscxx::PiecePlacementError::kPawnOnBackRank);

  chesscxx::PiecePlacement::PieceArray array;
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
