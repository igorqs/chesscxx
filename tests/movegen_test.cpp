#include <chesscxx/file.h>
#include <chesscxx/game.h>
#include <chesscxx/movegen.h>
#include <chesscxx/parse.h>
#include <chesscxx/piece_type.h>
#include <chesscxx/rank.h>
#include <chesscxx/san_move.h>
#include <chesscxx/square.h>
#include <chesscxx/uci_move.h>
#include <gtest/gtest.h>
#include <yaml-cpp/yaml.h>

#include <format>
#include <magic_enum/magic_enum.hpp>
#include <ostream>
#include <ranges>
#include <string_view>
#include <unordered_set>
#include <vector>

#include "test_helper.h"  // IWYU pragma: keep

constexpr auto kAllOptPromotions = chesscxx::testing::make_optional_enum_array<
    chesscxx::PromotablePieceType>();

constexpr auto kAllSquares =
    std::views::cartesian_product(magic_enum::enum_values<chesscxx::Rank>(),
                                  magic_enum::enum_values<chesscxx::File>()) |
    std::views::transform([](const auto& product) {
      const auto& [rank, file] = product;
      return chesscxx::Square(file, rank);
    });

constexpr auto kUciMoves =
    std::views::cartesian_product(kAllSquares, kAllSquares, kAllOptPromotions) |
    std::views::transform([](const auto& product) {
      const auto& [origin, destination, promotion] = product;
      return chesscxx::UciMove(origin, destination, promotion);
    });

class MovegenFixture {
 public:
  void set_input(std::string_view raw) {
    raw_ = raw;
    ASSERT_TRUE(chesscxx::parse<chesscxx::Game>(raw)) << std::format(
        "{} {}", raw, chesscxx::parse<chesscxx::Game>(raw).error());
    game_ = chesscxx::parse<chesscxx::Game>(raw).value();
  }
  void add_san_move(chesscxx::SanMove san_move) { san_moves_.insert(san_move); }
  void add_uci_move(chesscxx::UciMove uci_move) { uci_moves_.insert(uci_move); }

  auto raw() const -> const std::string& { return raw_; }
  auto game() const -> const chesscxx::Game& { return game_; }
  auto san_moves() const -> const std::unordered_set<chesscxx::SanMove>& {
    return san_moves_;
  }
  auto uci_moves() const -> const std::unordered_set<chesscxx::UciMove>& {
    return uci_moves_;
  }

  friend void PrintTo(const MovegenFixture& fixture, std::ostream* output) {
    *output << std::format("{} {} {}", fixture.raw_, fixture.san_moves_,
                           fixture.uci_moves_);
  }

 private:
  std::string raw_;
  chesscxx::Game game_;
  std::unordered_set<chesscxx::SanMove> san_moves_;
  std::unordered_set<chesscxx::UciMove> uci_moves_;
};

template <>
struct YAML::convert<MovegenFixture> {
  static auto decode(const Node& node, MovegenFixture& rhs) -> bool {
    rhs.set_input(node[0].as<std::string>());

    for (const auto& san_node : node[1]) {
      rhs.add_san_move(
          chesscxx::parse<chesscxx::SanMove>(san_node.as<std::string>())
              .value());
    }

    for (const auto& uci_node : node[2]) {
      rhs.add_uci_move(
          chesscxx::parse<chesscxx::UciMove>(uci_node.as<std::string>())
              .value());
    }

    return true;
  }
};

namespace {
auto GetConfig() { return YAML::LoadFile("data/movegen.yaml"); }

auto GetMovegenFixtures() {
  return GetConfig()["fixtures"].as<std::vector<MovegenFixture>>();
}

auto GetOverflowMovegenFixtures() {
  return GetConfig()["overflow_fixtures"].as<std::vector<MovegenFixture>>();
}
}  // namespace

class MovegenSuite : public ::testing::TestWithParam<MovegenFixture> {};
INSTANTIATE_TEST_SUITE_P(MovegenTest, MovegenSuite,
                         ::testing::ValuesIn(GetMovegenFixtures()));

class OverflowMovegenSuite : public ::testing::TestWithParam<MovegenFixture> {};
INSTANTIATE_TEST_SUITE_P(MovegenTest, OverflowMovegenSuite,
                         ::testing::ValuesIn(GetOverflowMovegenFixtures()));

TEST_P(MovegenSuite, GenerateLegalMovesCorrectly) {
  const auto& fixture = GetParam();

  auto san_moves = fixture.san_moves();

  for (auto move : chesscxx::legalSanMoves(fixture.game())) {
    EXPECT_TRUE(san_moves.contains(move)) << std::format("missing {}", move);
    san_moves.erase(move);
  }

  EXPECT_TRUE(san_moves.empty());

  auto uci_moves = fixture.uci_moves();

  for (auto move : chesscxx::legalUciMoves(fixture.game())) {
    EXPECT_TRUE(uci_moves.contains(move)) << std::format("missing {}", move);
    uci_moves.erase(move);
  }

  EXPECT_TRUE(uci_moves.empty());
}

TEST_P(MovegenSuite, GeneratedLegalMovesAndLegalMovesAreConsistent) {
  const auto& fixture = GetParam();

  const auto& uci_moves = fixture.uci_moves();
  auto game = fixture.game();

  for (const auto& move : kUciMoves) {
    auto result = game.move(move);
    EXPECT_EQ(result.has_value(), uci_moves.contains(move))
        << std::format("{:fen} {} {} {}", game, move, result.has_value(),
                       uci_moves.contains(move));
    if (result) game.undoMove();
  }
}

TEST_P(OverflowMovegenSuite, GenerateLegalMovesCorrectly) {
  const auto& fixture = GetParam();

  auto san_moves = fixture.san_moves();

  for (auto move : chesscxx::legalSanMoves(fixture.game())) {
    EXPECT_TRUE(san_moves.contains(move)) << std::format("missing {}", move);
    san_moves.erase(move);
  }

  EXPECT_TRUE(san_moves.empty());

  auto uci_moves = fixture.uci_moves();

  for (auto move : chesscxx::legalUciMoves(fixture.game())) {
    EXPECT_TRUE(uci_moves.contains(move)) << std::format("missing {}", move);
    uci_moves.erase(move);
  }

  EXPECT_TRUE(uci_moves.empty());
}
