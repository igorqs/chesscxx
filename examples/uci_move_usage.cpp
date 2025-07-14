#include <chesskit/file.h>
#include <chesskit/parse.h>
#include <chesskit/piece_type.h>
#include <chesskit/rank.h>
#include <chesskit/square.h>
#include <chesskit/uci_move.h>

#include <print>
#include <string_view>

namespace {
void printErrorOrValue(auto parsedValue) {
  if (parsedValue) {
    std::println("{}", parsedValue.value());
  } else {
    std::println("{}", parsedValue.error());
  }
}

void parseAndPrint(std::string_view sv) {
  auto parsedUciMove = chesskit::parse<chesskit::UciMove>(sv);
  printErrorOrValue(parsedUciMove);
}
}  // namespace

int main() {
  chesskit::UciMove uci = {
      .origin = chesskit::Square(chesskit::File::kE, chesskit::Rank::k2),
      .destination = chesskit::Square(chesskit::File::kE, chesskit::Rank::k4),
  };
  std::println("{}", uci);

  uci = {
      .origin = chesskit::Square(chesskit::File::kA, chesskit::Rank::k7),
      .destination = chesskit::Square(chesskit::File::kA, chesskit::Rank::k8),
      .promotion = chesskit::PromotablePieceType::kQueen,
  };
  std::println("{}", uci);

  parseAndPrint("e4");
  parseAndPrint("e2e4");
  parseAndPrint("a7a8q");
  parseAndPrint("a7a8k");
  parseAndPrint("e8=Q");
  parseAndPrint("e2e4+");
}
