#include <chesscxx/file.h>
#include <chesscxx/parse.h>
#include <chesscxx/piece_type.h>
#include <chesscxx/rank.h>
#include <chesscxx/square.h>
#include <chesscxx/uci_move.h>

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
  auto parsed_uci_move = chesscxx::parse<chesscxx::UciMove>(str);
  printErrorOrValue(parsed_uci_move);
}
}  // namespace

auto main() -> int {
  chesscxx::UciMove uci = {
      .origin = chesscxx::Square(chesscxx::File::kE, chesscxx::Rank::k2),
      .destination = chesscxx::Square(chesscxx::File::kE, chesscxx::Rank::k4),
  };
  std::println("{}", uci);

  uci = {
      .origin = chesscxx::Square(chesscxx::File::kA, chesscxx::Rank::k7),
      .destination = chesscxx::Square(chesscxx::File::kA, chesscxx::Rank::k8),
      .promotion = chesscxx::PromotablePieceType::kQueen,
  };
  std::println("{}", uci);

  parseAndPrint("e4");
  parseAndPrint("e2e4");
  parseAndPrint("a7a8q");
  parseAndPrint("a7a8k");
  parseAndPrint("e8=Q");
  parseAndPrint("e2e4+");
}
