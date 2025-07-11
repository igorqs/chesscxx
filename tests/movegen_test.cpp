#include <chesskit/file.h>
#include <chesskit/game.h>
#include <chesskit/movegen.h>
#include <chesskit/parse.h>
#include <chesskit/piece_type.h>
#include <chesskit/rank.h>
#include <chesskit/san_move.h>
#include <chesskit/square.h>
#include <chesskit/uci_move.h>
#include <gtest/gtest.h>
#include <yaml-cpp/yaml.h>

#include <format>
#include <magic_enum/magic_enum.hpp>
#include <ranges>
#include <string_view>
#include <unordered_set>
#include <vector>

#include "test_helper.h"  // IWYU pragma: keep

constexpr auto kAllOptPromotions = chesskit::testing::make_optional_enum_array<
    chesskit::PromotablePieceType>();

constexpr auto kAllSquares =
    std::views::cartesian_product(magic_enum::enum_values<chesskit::Rank>(),
                                  magic_enum::enum_values<chesskit::File>()) |
    std::views::transform([](const auto& product) {
      const auto& [rank, file] = product;
      return chesskit::Square(file, rank);
    });

constexpr auto kUciMoves =
    std::views::cartesian_product(kAllSquares, kAllSquares, kAllOptPromotions) |
    std::views::transform([](const auto& product) {
      const auto& [origin, destination, promotion] = product;
      return chesskit::UciMove(origin, destination, promotion);
    });

class MovegenFixture {
 public:
  void set_input(std::string_view raw) {
    raw_ = raw;
    ASSERT_TRUE(chesskit::parse<chesskit::Game>(raw)) << std::format(
        "{} {}", raw, chesskit::parse<chesskit::Game>(raw).error());
    game_ = chesskit::parse<chesskit::Game>(raw).value();
  }
  void add_san_move(chesskit::SanMove san_move) { san_moves_.insert(san_move); }
  void add_uci_move(chesskit::UciMove uci_move) { uci_moves_.insert(uci_move); }

  const std::string& raw() const { return raw_; }
  const chesskit::Game& game() const { return game_; }
  const std::unordered_set<chesskit::SanMove>& san_moves() const {
    return san_moves_;
  }
  const std::unordered_set<chesskit::UciMove>& uci_moves() const {
    return uci_moves_;
  }

 private:
  std::string raw_;
  chesskit::Game game_;
  std::unordered_set<chesskit::SanMove> san_moves_;
  std::unordered_set<chesskit::UciMove> uci_moves_;
};

template <>
struct YAML::convert<MovegenFixture> {
  static bool decode(const Node& node, MovegenFixture& rhs) {
    rhs.set_input(node[0].as<std::string>());

    for (const auto& san_node : node[1]) {
      rhs.add_san_move(
          chesskit::parse<chesskit::SanMove>(san_node.as<std::string>())
              .value());
    }

    for (const auto& uci_node : node[2]) {
      rhs.add_uci_move(
          chesskit::parse<chesskit::UciMove>(uci_node.as<std::string>())
              .value());
    }

    return true;
  }
};

auto GetConfig() { return YAML::LoadFile("data/movegen.yaml"); }

auto GetMovegenFixtures() {
  return GetConfig()["fixtures"].as<std::vector<MovegenFixture>>();
}

auto GetOverflowMovegenFixtures() {
  return GetConfig()["overflow_fixtures"].as<std::vector<MovegenFixture>>();
}

class MovegenSuite : public ::testing::TestWithParam<MovegenFixture> {};
INSTANTIATE_TEST_SUITE_P(MovegenTest, MovegenSuite,
                         ::testing::ValuesIn(GetMovegenFixtures()));

class OverflowMovegenSuite : public ::testing::TestWithParam<MovegenFixture> {};
INSTANTIATE_TEST_SUITE_P(MovegenTest, OverflowMovegenSuite,
                         ::testing::ValuesIn(GetOverflowMovegenFixtures()));

TEST_P(MovegenSuite, GenerateLegalMovesCorrectly) {
  const auto& fixture = GetParam();

  auto san_moves = fixture.san_moves();

  for (auto move : chesskit::legalSanMoves(fixture.game())) {
    EXPECT_TRUE(san_moves.contains(move));
    san_moves.erase(move);
  }

  EXPECT_TRUE(san_moves.empty());

  auto uci_moves = fixture.uci_moves();

  for (auto move : chesskit::legalUciMoves(fixture.game())) {
    EXPECT_TRUE(uci_moves.contains(move));
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
    EXPECT_EQ(result.has_value(), uci_moves.contains(move));
    if (result) game.undoMove();
  }
}

TEST_P(OverflowMovegenSuite, GenerateLegalMovesCorrectly) {
  const auto& fixture = GetParam();

  auto san_moves = fixture.san_moves();

  for (auto move : chesskit::legalSanMoves(fixture.game())) {
    EXPECT_TRUE(san_moves.contains(move));
    san_moves.erase(move);
  }

  EXPECT_TRUE(san_moves.empty());

  auto uci_moves = fixture.uci_moves();

  for (auto move : chesskit::legalUciMoves(fixture.game())) {
    EXPECT_TRUE(uci_moves.contains(move));
    uci_moves.erase(move);
  }

  EXPECT_TRUE(uci_moves.empty());
}
