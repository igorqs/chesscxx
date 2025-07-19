#include <chesskit/file.h>
#include <chesskit/parse.h>
#include <chesskit/piece_type.h>
#include <chesskit/rank.h>
#include <chesskit/square.h>
#include <chesskit/uci_move.h>

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
  auto parsed_uci_move = chesskit::parse<chesskit::UciMove>(str);
  printErrorOrValue(parsed_uci_move);
}
}  // namespace

auto main() -> int {
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
