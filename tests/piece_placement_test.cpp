#include <chesscxx/color.h>
#include <chesscxx/parse.h>
#include <chesscxx/parse_error.h>
#include <chesscxx/piece.h>
#include <chesscxx/piece_placement.h>
#include <chesscxx/piece_placement_error.h>
#include <chesscxx/piece_type.h>
#include <chesscxx/square.h>
#include <gtest/gtest.h>
#include <yaml-cpp/yaml.h>

#include <algorithm>
#include <array>
#include <bitset>
#include <cstddef>
#include <expected>
#include <format>
#include <functional>
#include <magic_enum/magic_enum.hpp>
#include <optional>
#include <string_view>
#include <tuple>
#include <unordered_map>
#include <vector>

#include "test_helper.h"

namespace chesscxx {
template <>
class Parser<PiecePlacement::PieceArray, const char*, parse_as::Default> {
 public:
  static constexpr auto parse(const char* begin, const char* end)
      -> std::expected<ParseResult<PiecePlacement::PieceArray, const char*>,
                       ParseError> {
    return internal::parsePieceArray(begin, end);
  }
};
}  // namespace chesscxx

class ValidFixture {
 public:
  void set_input(std::string_view raw) {
    raw_ = raw;
    piece_array_ =
        chesscxx::parse<chesscxx::PiecePlacement::PieceArray>(raw).value();
    piece_placement_ = chesscxx::parse<chesscxx::PiecePlacement>(raw).value();
  }
  auto raw() const -> const std::string& { return raw_; }
  auto piece_array() const -> const chesscxx::PiecePlacement::PieceArray& {
    return piece_array_;
  }
  auto piece_placement() const -> const chesscxx::PiecePlacement& {
    return piece_placement_;
  }

 private:
  std::string raw_;
  chesscxx::PiecePlacement::PieceArray piece_array_;
  chesscxx::PiecePlacement piece_placement_;
};

template <>
struct YAML::convert<ValidFixture> {
  static auto decode(const Node& node, ValidFixture& rhs) -> bool {
    rhs.set_input(node.as<std::string>());
    return true;
  }
};

class InvalidPieceArrayFixture {
 public:
  void set_input(std::string_view raw) {
    raw_ = raw;
    piece_array_ =
        chesscxx::parse<chesscxx::PiecePlacement::PieceArray>(raw).value();
  }
  void set_error(chesscxx::PiecePlacementError error) { error_ = error; }
  [[nodiscard]] auto raw() const -> const std::string& { return raw_; }
  [[nodiscard]] auto piece_array() const
      -> const chesscxx::PiecePlacement::PieceArray& {
    return piece_array_;
  }
  [[nodiscard]] auto error() const -> const chesscxx::PiecePlacementError& {
    return error_;
  }

 private:
  std::string raw_;
  chesscxx::PiecePlacement::PieceArray piece_array_;
  chesscxx::PiecePlacementError error_{};
};

template <>
struct YAML::convert<InvalidPieceArrayFixture> {
  static auto decode(const Node& node, InvalidPieceArrayFixture& rhs) -> bool {
    rhs.set_input(node[0].as<std::string>());

    auto error = magic_enum::enum_cast<chesscxx::PiecePlacementError>(
        node[1].as<std::string>());
    if (!error.has_value()) return false;
    rhs.set_error(*error);

    return true;
  }
};

class InvalidFixture {
 public:
  void set_input(std::string_view raw) { raw_ = raw; }
  void set_error(chesscxx::ParseError error) { error_ = error; }
  [[nodiscard]] auto raw() const -> const std::string& { return raw_; }
  [[nodiscard]] auto error() const -> const chesscxx::ParseError& {
    return error_;
  }

 private:
  std::string raw_;
  chesscxx::ParseError error_{};
};

template <>
struct YAML::convert<InvalidFixture> {
  static auto decode(const Node& node, InvalidFixture& rhs) -> bool {
    rhs.set_input(node[0].as<std::string>());
    auto error =
        magic_enum::enum_cast<chesscxx::ParseError>(node[1].as<std::string>());
    if (!error.has_value()) return false;
    rhs.set_error(*error);

    return true;
  }
};

namespace {
auto GetConfig() { return YAML::LoadFile("data/piece_placement.yaml"); }

auto GetDefault() { return GetConfig()["default"].as<ValidFixture>(); }

auto GetComplexInput() {
  return GetConfig()["complex_input"].as<ValidFixture>();
}

auto GetValidInputs() {
  return GetConfig()["valid_inputs"].as<std::vector<ValidFixture>>();
}

auto BuildValidInputPairs() {
  return chesscxx::testing::make_distinct_pairs(GetValidInputs());
}

auto GetInvalidPieceArrayInputs() {
  return GetConfig()["invalid_piece_array_inputs"]
      .as<std::vector<InvalidPieceArrayFixture>>();
}

auto GetInvalidInputs() {
  return GetConfig()["invalid_inputs"].as<std::vector<InvalidFixture>>();
}
}  // namespace

class ValidInputSuite : public ::testing::TestWithParam<ValidFixture> {};
INSTANTIATE_TEST_SUITE_P(PiecePlacementTest, ValidInputSuite,
                         ::testing::ValuesIn(GetValidInputs()));

class ValidInputPairSuite
    : public ::testing::TestWithParam<std::tuple<ValidFixture, ValidFixture>> {
};
INSTANTIATE_TEST_SUITE_P(PiecePlacementTest, ValidInputPairSuite,
                         ::testing::ValuesIn(BuildValidInputPairs()));

class InvalidPieceArraySuite
    : public ::testing::TestWithParam<InvalidPieceArrayFixture> {};
INSTANTIATE_TEST_SUITE_P(PiecePlacementTest, InvalidPieceArraySuite,
                         ::testing::ValuesIn(GetInvalidPieceArrayInputs()));

class InvalidInputSuite : public ::testing::TestWithParam<InvalidFixture> {};
INSTANTIATE_TEST_SUITE_P(PiecePlacementTest, InvalidInputSuite,
                         ::testing::ValuesIn(GetInvalidInputs()));

TEST_P(ValidInputSuite, ParsesSuccessfully) {
  const auto& fen = GetParam().raw();
  auto parsed_pp = chesscxx::parse<chesscxx::PiecePlacement>(fen);
  ASSERT_TRUE(parsed_pp);
}

TEST_P(ValidInputSuite, ConstructsFromPieceArraySuccessfully) {
  const auto& array = GetParam().piece_array();
  auto piece_placement = chesscxx::PiecePlacement::fromPieceArray(array);
  ASSERT_TRUE(piece_placement);
  EXPECT_EQ(piece_placement->pieceArray(), array);
}

TEST_P(ValidInputSuite, ComparesEqual) {
  auto lhs = GetParam().piece_placement();
  auto rhs = GetParam().piece_placement();
  EXPECT_EQ(lhs, lhs);
  EXPECT_EQ(lhs, rhs);
}

TEST_P(ValidInputSuite, PieceArrayAndPieceLocationsAreConsistent) {
  const auto& piece_placement = GetParam().piece_placement();
  std::bitset<chesscxx::kNumSquares> seen;

  for (const auto& [color, locationsByPieceType] :
       piece_placement.pieceLocations()) {
    for (const auto& [type, locations] : locationsByPieceType) {
      for (const chesscxx::Square& location : locations) {
        chesscxx::Piece const piece = {.type = type, .color = color};
        auto location_index = chesscxx::index(location);
        seen.set(location_index);
        EXPECT_EQ(piece_placement.pieceArray().at(location_index), piece);
      }
    }
  }

  for (size_t i = 0; i < chesscxx::kNumSquares; i++) {
    if (seen[i]) continue;

    EXPECT_EQ(piece_placement.pieceArray().at(i), std::nullopt);
  }
}

TEST_P(ValidInputSuite, RoundTripConversionIsSuccessful) {
  const auto& piece_placement = GetParam().piece_placement();
  EXPECT_EQ(piece_placement, chesscxx::parse<chesscxx::PiecePlacement>(
                                 std::format("{:fen}", piece_placement)));
}

TEST_P(ValidInputSuite, FormatProducesNonEmptyStrings) {
  const auto& piece_placement = GetParam().piece_placement();
  EXPECT_FALSE(std::format("{}", piece_placement).empty());
  EXPECT_FALSE(std::format("{:fen}", piece_placement).empty());
  EXPECT_FALSE(std::format("{:ascii}", piece_placement).empty());
  EXPECT_FALSE(std::format("{:lists}", piece_placement).empty());
}

TEST_P(ValidInputPairSuite, ComparesUnequal) {
  const auto& [lhs, rhs] = GetParam();
  EXPECT_NE(lhs.piece_placement(), rhs.piece_placement());
}

TEST_P(InvalidPieceArraySuite, FromPieceArrayReturnsCorrectError) {
  const auto& fixture = GetParam();
  EXPECT_EQ(
      chesscxx::PiecePlacement::fromPieceArray(fixture.piece_array()).error(),
      fixture.error());
}

TEST_P(InvalidPieceArraySuite, ParseReturnsCorrectError) {
  const auto& fen = GetParam().raw();
  EXPECT_EQ(chesscxx::parse<chesscxx::PiecePlacement>(fen).error(),
            chesscxx::ParseError::kInvalidPiecePlacement);
}

TEST_P(InvalidInputSuite, ParseHandlesInvalidInputCorrectly) {
  const auto& fixture = GetParam();
  EXPECT_EQ(chesscxx::parse<chesscxx::PiecePlacement>(fixture.raw()).error(),
            fixture.error());
}

TEST(PiecePlacementTest, DefaultConstructionCreatesDefaultPieceArray) {
  chesscxx::PiecePlacement const piece_placement;
  EXPECT_EQ(GetDefault().piece_array(), piece_placement.pieceArray());
}

TEST(PiecePlacementTest, FormatComplexInputCorrectly) {
  auto complex = GetComplexInput();
  const auto& piece_placement = complex.piece_placement();
  EXPECT_EQ(std::format("{}", piece_placement), complex.raw());
  EXPECT_EQ(std::format("{:fen}", piece_placement), complex.raw());
  EXPECT_EQ(std::format("{:ascii}", piece_placement),
            "r..qk..Q\n"
            "pppppppp\n"
            ".nb.Pbnr\n"
            "........\n"
            "R..Pp...\n"
            "........\n"
            "........\n"
            ".NBQKBNR");
  EXPECT_EQ(
      std::format("{:lists}", piece_placement),
      "{ white king: [e1], white bishops: [f1, c1], white knights: [g1, "
      "b1], white rooks: [h1, a4], white pawns: [d4, e6], white queens: [d1, "
      "h8], black bishops: [f6, c6], black knights: [g6, b6], black "
      "pawns: [h7, g7, f7, e7, d7, e4, c7, b7, a7], black king: [e8], "
      "black queen: [d8], black rooks: [h6, a8] }");
}

TEST(PiecePlacementTest, ParseComplexInputCorrectly) {
  using enum chesscxx::PieceType;
  using enum chesscxx::Color;
  using chesscxx::Piece;

  static constexpr auto kWRook = Piece(kRook, kWhite);
  static constexpr auto kWKnight = Piece(kKnight, kWhite);
  static constexpr auto kWBishop = Piece(kBishop, kWhite);
  static constexpr auto kWQueen = Piece(kQueen, kWhite);
  static constexpr auto kWKing = Piece(kKing, kWhite);
  static constexpr auto kWPawn = Piece(kPawn, kWhite);
  static constexpr auto kBRook = Piece(kRook, kBlack);
  static constexpr auto kBKnight = Piece(kKnight, kBlack);
  static constexpr auto kBBishop = Piece(kBishop, kBlack);
  static constexpr auto kBQueen = Piece(kQueen, kBlack);
  static constexpr auto kBKing = Piece(kKing, kBlack);
  static constexpr auto kBPawn = Piece(kPawn, kBlack);
  static constexpr auto kEmpty = std::nullopt;

  static constexpr std::array<std::optional<Piece>, 64> kExpectedBoard = {
      // Rank 8: r2qk2Q
      kBRook, kEmpty, kEmpty, kBQueen, kBKing, kEmpty, kEmpty, kWQueen,
      // Rank 7: pppppppp
      kBPawn, kBPawn, kBPawn, kBPawn, kBPawn, kBPawn, kBPawn, kBPawn,
      // Rank 6: 1nb1Pbnr
      kEmpty, kBKnight, kBBishop, kEmpty, kWPawn, kBBishop, kBKnight, kBRook,
      // Rank 5: 8 (empty)
      kEmpty, kEmpty, kEmpty, kEmpty, kEmpty, kEmpty, kEmpty, kEmpty,
      // Rank 4: R2Pp3
      kWRook, kEmpty, kEmpty, kWPawn, kBPawn, kEmpty, kEmpty, kEmpty,
      // Rank 3: 8 (empty)
      kEmpty, kEmpty, kEmpty, kEmpty, kEmpty, kEmpty, kEmpty, kEmpty,
      // Rank 2: 8 (empty)
      kEmpty, kEmpty, kEmpty, kEmpty, kEmpty, kEmpty, kEmpty, kEmpty,
      // Rank 1: 1NBQKBNR
      kEmpty, kWKnight, kWBishop, kWQueen, kWKing, kWBishop, kWKnight, kWRook};

  auto parsed_pp =
      chesscxx::parse<chesscxx::PiecePlacement>(GetComplexInput().raw());
  ASSERT_TRUE(parsed_pp);
  EXPECT_EQ(parsed_pp->pieceArray(), kExpectedBoard);
}

TEST(PiecePlacementTest, HashProducesFewCollisions) {
  // Adjust based on expectations for the test set.
  constexpr int kMaxCollisions = 1;
  std::unordered_map<size_t, std::vector<chesscxx::PiecePlacement>>
      hash_counter;

  std::ranges::for_each(GetValidInputs(), [&](const auto& fixture) {
    const auto& piece_placement = fixture.piece_placement();
    auto hash = std::hash<chesscxx::PiecePlacement>{}(piece_placement);
    hash_counter[hash].push_back(piece_placement);
    auto collisions = hash_counter[hash].size();
    EXPECT_LE(collisions, kMaxCollisions) << std::format(
        "piece_placement={}, hash_counter={}", piece_placement, hash_counter);
  });
}

TEST(PiecePlacementTest, HashProducesExpectedCollision) {
  auto lhs_pp =
      chesscxx::parse<chesscxx::PiecePlacement>("1RB1k3/8/8/8/8/8/8/4K3");
  auto rhs_pp =
      chesscxx::parse<chesscxx::PiecePlacement>("1rN1k3/8/8/8/8/8/8/4K3");
  ASSERT_TRUE(lhs_pp);
  ASSERT_TRUE(rhs_pp);
  EXPECT_NE(*lhs_pp, *rhs_pp);
  EXPECT_EQ(std::hash<chesscxx::PiecePlacement>{}(*lhs_pp),
            std::hash<chesscxx::PiecePlacement>{}(*rhs_pp))
      << std::format("lhs_pp:{} rhs_pp{}", *lhs_pp, *rhs_pp);
}

TEST(PiecePlacementTest, FormatProducesUniqueStrings) {
  using CollisionMap =
      std::unordered_map<std::string, std::vector<chesscxx::PiecePlacement>>;
  CollisionMap fmt_collisions;
  CollisionMap ascii_fmt_collisions;
  CollisionMap lists_fmt_collisions;

  auto assert_unique_insertion = [](auto& map, const auto& key,
                                    const auto& val) {
    map[key].push_back(val);
    EXPECT_EQ(map[key].size(), 1)
        << std::format("piece_placement={}, map={}", val, map);
  };

  std::ranges::for_each(GetValidInputs(), [&](const auto& fixture) {
    const auto& piece_placement = fixture.piece_placement();
    assert_unique_insertion(fmt_collisions, std::format("{}", piece_placement),
                            piece_placement);
    assert_unique_insertion(ascii_fmt_collisions,
                            std::format("{:ascii}", piece_placement),
                            piece_placement);
    assert_unique_insertion(lists_fmt_collisions,
                            std::format("{:lists}", piece_placement),
                            piece_placement);
  });
}
