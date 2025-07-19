#include <chesskit/parse.h>
#include <chesskit/piece.h>
#include <chesskit/piece_placement.h>
#include <chesskit/square.h>

#include <bitset>
#include <cassert>
#include <cstddef>
#include <optional>
#include <print>
#include <string_view>

namespace {
void printErrorOrValue(auto parsed_value) {
  if (parsed_value) {
    std::println("{}", parsed_value.value());
  } else {
    std::println("{}", parsed_value.error());
  }
}

void parseAndPrint(std::string_view str) {
  auto parsed_piece_placement = chesskit::parse<chesskit::PiecePlacement>(str);
  printErrorOrValue(parsed_piece_placement);
}
}  // namespace

auto main() -> int {
  chesskit::PiecePlacement piece_placement;
  std::println("{}\n", piece_placement);
  std::println("{:fen}\n", piece_placement);
  std::println("{:ascii}\n", piece_placement);
  std::println("{:lists}\n", piece_placement);

  std::bitset<chesskit::kNumSquares> seen;

  for (const auto& [color, locationsByPieceType] :
       piece_placement.pieceLocations()) {
    for (const auto& [type, locations] : locationsByPieceType) {
      for (const chesskit::Square& location : locations) {
        chesskit::Piece const piece = {.type = type, .color = color};
        auto location_index = chesskit::index(location);
        seen.set(location_index);
        assert(piece_placement.pieceArray().at(location_index) == piece);
      }
    }
  }

  for (size_t i = 0; i < chesskit::kNumSquares; i++) {
    if (!seen[i]) assert(piece_placement.pieceArray().at(i) == std::nullopt);
  }

  parseAndPrint("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR");
  parseAndPrint("8/8/8/8/8/8/8/8");
  parseAndPrint("xnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR");
  parseAndPrint("rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR");
  parseAndPrint("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
  parseAndPrint("rnbqkbnr/ppp/8/8/8/8/PPPPPPPP/RNBQKBNR");
  parseAndPrint("rnbqkbnr/7/8/8/8/8/PPPPPPPP/RNBQKBNR");
  parseAndPrint("rnbqkbnr/9/8/8/8/8/PPPPPPPP/RNBQKBNR");
}
