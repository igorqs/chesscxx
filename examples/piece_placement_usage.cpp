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

void printErrorOrValue(auto parsedValue) {
  if (parsedValue) {
    std::println("{}", parsedValue.value());
  } else {
    std::println("{}", parsedValue.error());
  }
}

void parseAndPrint(std::string_view sv) {
  auto parsedPiecePlacement = chesskit::parse<chesskit::PiecePlacement>(sv);
  printErrorOrValue(parsedPiecePlacement);
}

int main() {
  chesskit::PiecePlacement pp;
  std::println("{}\n", pp);
  std::println("{:fen}\n", pp);
  std::println("{:ascii}\n", pp);
  std::println("{:lists}\n", pp);

  std::bitset<chesskit::kNumSquares> seen;

  for (const auto& [color, locationsByPieceType] : pp.pieceLocations()) {
    for (const auto& [type, locations] : locationsByPieceType) {
      for (const chesskit::Square& location : locations) {
        chesskit::Piece piece = {type, color};
        auto location_index = chesskit::index(location);
        seen.set(location_index);
        assert(pp.pieceArray().at(location_index) == piece);
      }
    }
  }

  for (size_t i = 0; i < chesskit::kNumSquares; i++) {
    if (!seen[i]) assert(pp.pieceArray().at(i) == std::nullopt);
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
