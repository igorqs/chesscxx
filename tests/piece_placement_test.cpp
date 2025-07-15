#include <chesskit/color.h>
#include <chesskit/file.h>
#include <chesskit/parse.h>
#include <chesskit/parse_error.h>
#include <chesskit/piece.h>
#include <chesskit/piece_placement.h>
#include <chesskit/piece_placement_error.h>
#include <chesskit/piece_type.h>
#include <chesskit/rank.h>
#include <chesskit/square.h>
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

namespace chesskit {
template <>
class Parser<PiecePlacement::PieceArray, const char*, parse_as::Default> {
 public:
  constexpr auto parse(const char* begin, const char* end)
      -> std::expected<ParseResult<PiecePlacement::PieceArray, const char*>,
                       ParseError> {
    return internal::parsePieceArray(begin, end);
  }
};
}  // namespace chesskit

class ValidFixture {
 public:
  void set_input(std::string_view raw) {
    raw_ = raw;
    piece_array_ =
        chesskit::parse<chesskit::PiecePlacement::PieceArray>(raw).value();
    piece_placement_ = chesskit::parse<chesskit::PiecePlacement>(raw).value();
  }
  auto raw() const -> const std::string& { return raw_; }
  auto piece_array() const -> const chesskit::PiecePlacement::PieceArray& {
    return piece_array_;
  }
  auto piece_placement() const -> const chesskit::PiecePlacement& {
    return piece_placement_;
  }

 private:
  std::string raw_;
  chesskit::PiecePlacement::PieceArray piece_array_;
  chesskit::PiecePlacement piece_placement_;
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
        chesskit::parse<chesskit::PiecePlacement::PieceArray>(raw).value();
  }
  void set_error(chesskit::PiecePlacementError error) { error_ = error; }
  [[nodiscard]] auto raw() const -> const std::string& { return raw_; }
  [[nodiscard]] auto piece_array() const
      -> const chesskit::PiecePlacement::PieceArray& {
    return piece_array_;
  }
  [[nodiscard]] auto error() const -> const chesskit::PiecePlacementError& {
    return error_;
  }

 private:
  std::string raw_;
  chesskit::PiecePlacement::PieceArray piece_array_;
  chesskit::PiecePlacementError error_;
};

template <>
struct YAML::convert<InvalidPieceArrayFixture> {
  static auto decode(const Node& node, InvalidPieceArrayFixture& rhs) -> bool {
    rhs.set_input(node[0].as<std::string>());

    auto error = magic_enum::enum_cast<chesskit::PiecePlacementError>(
        node[1].as<std::string>());
    if (!error.has_value()) return false;
    rhs.set_error(*error);

    return true;
  }
};

class InvalidFixture {
 public:
  void set_input(std::string_view raw) { raw_ = raw; }
  void set_error(chesskit::ParseError error) { error_ = error; }
  [[nodiscard]] auto raw() const -> const std::string& { return raw_; }
  [[nodiscard]] auto error() const -> const chesskit::ParseError& {
    return error_;
  }

 private:
  std::string raw_;
  chesskit::ParseError error_;
};

template <>
struct YAML::convert<InvalidFixture> {
  static auto decode(const Node& node, InvalidFixture& rhs) -> bool {
    rhs.set_input(node[0].as<std::string>());
    auto error =
        magic_enum::enum_cast<chesskit::ParseError>(node[1].as<std::string>());
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
  return chesskit::testing::make_distinct_pairs(GetValidInputs());
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
  auto parsed_pp = chesskit::parse<chesskit::PiecePlacement>(fen);
  ASSERT_TRUE(parsed_pp);
}

TEST_P(ValidInputSuite, ConstructsFromPieceArraySuccessfully) {
  const auto& array = GetParam().piece_array();
  auto pp = chesskit::PiecePlacement::fromPieceArray(array);
  ASSERT_TRUE(pp);
  EXPECT_EQ(pp->pieceArray(), array);
}

TEST_P(ValidInputSuite, ComparesEqual) {
  auto lhs = GetParam().piece_placement();
  auto rhs = GetParam().piece_placement();
  EXPECT_EQ(lhs, lhs);
  EXPECT_EQ(lhs, rhs);
}

TEST_P(ValidInputSuite, PieceArrayAndPieceLocationsAreConsistent) {
  const auto& pp = GetParam().piece_placement();
  std::bitset<chesskit::kNumSquares> seen;

  for (const auto& [color, locationsByPieceType] : pp.pieceLocations()) {
    for (const auto& [type, locations] : locationsByPieceType) {
      for (const chesskit::Square& location : locations) {
        chesskit::Piece const piece = {.type = type, .color = color};
        auto location_index = chesskit::index(location);
        seen.set(location_index);
        EXPECT_EQ(pp.pieceArray().at(location_index), piece);
      }
    }
  }

  for (size_t i = 0; i < chesskit::kNumSquares; i++) {
    if (!seen[i]) EXPECT_EQ(pp.pieceArray().at(i), std::nullopt);
  }
}

TEST_P(ValidInputSuite, RoundTripConversionIsSuccessful) {
  const auto& pp = GetParam().piece_placement();
  EXPECT_EQ(
      pp, chesskit::parse<chesskit::PiecePlacement>(std::format("{:fen}", pp)));
}

TEST_P(ValidInputSuite, FormatProducesNonEmptyStrings) {
  const auto& pp = GetParam().piece_placement();
  EXPECT_FALSE(std::format("{}", pp).empty());
  EXPECT_FALSE(std::format("{:fen}", pp).empty());
  EXPECT_FALSE(std::format("{:ascii}", pp).empty());
  EXPECT_FALSE(std::format("{:lists}", pp).empty());
}

TEST_P(ValidInputPairSuite, ComparesUnequal) {
  const auto& [lhs, rhs] = GetParam();
  EXPECT_NE(lhs.piece_placement(), rhs.piece_placement());
}

TEST_P(InvalidPieceArraySuite, FromPieceArrayReturnsCorrectError) {
  const auto& fixture = GetParam();
  EXPECT_EQ(
      chesskit::PiecePlacement::fromPieceArray(fixture.piece_array()).error(),
      fixture.error());
}

TEST_P(InvalidPieceArraySuite, ParseReturnsCorrectError) {
  const auto& fen = GetParam().raw();
  EXPECT_EQ(chesskit::parse<chesskit::PiecePlacement>(fen).error(),
            chesskit::ParseError::kInvalidPiecePlacement);
}

TEST_P(InvalidInputSuite, ParseHandlesInvalidInputCorrectly) {
  const auto& fixture = GetParam();
  EXPECT_EQ(chesskit::parse<chesskit::PiecePlacement>(fixture.raw()).error(),
            fixture.error());
}

TEST(PiecePlacementTest, DefaultConstructionCreatesDefaultPieceArray) {
  chesskit::PiecePlacement const piece_placement;
  EXPECT_EQ(GetDefault().piece_array(), piece_placement.pieceArray());
}

TEST(PiecePlacementTest, FormatComplexInputCorrectly) {
  auto complex = GetComplexInput();
  const auto& pp = complex.piece_placement();
  EXPECT_EQ(std::format("{}", pp), complex.raw());
  EXPECT_EQ(std::format("{:fen}", pp), complex.raw());
  EXPECT_EQ(std::format("{:ascii}", pp),
            "r..qk..Q\n"
            "pppppppp\n"
            ".nb.Pbnr\n"
            "........\n"
            "R..Pp...\n"
            "........\n"
            "........\n"
            ".NBQKBNR");
  EXPECT_EQ(
      std::format("{:lists}", pp),
      "{ white king: [e1], white bishops: [f1, c1], white knights: [g1, "
      "b1], white rooks: [h1, a4], white pawns: [d4, e6], white queens: [d1, "
      "h8], black bishops: [f6, c6], black knights: [g6, b6], black "
      "pawns: [h7, g7, f7, e7, d7, e4, c7, b7, a7], black king: [e8], "
      "black queen: [d8], black rooks: [h6, a8] }");
}

TEST(PiecePlacementTest, ParseComplexInputCorrectly) {
  using enum chesskit::PieceType;
  using enum chesskit::Color;
  using enum chesskit::Rank;
  using enum chesskit::File;
  using chesskit::Piece, chesskit::Square, chesskit::index;

  auto parsed_pp =
      chesskit::parse<chesskit::PiecePlacement>(GetComplexInput().raw());
  ASSERT_TRUE(parsed_pp);

  const auto& array = parsed_pp->pieceArray();

  // Rank 8: r2qk2Q
  EXPECT_EQ(array[index(Square(kA, k8))], Piece(kRook, kBlack));
  EXPECT_EQ(array[index(Square(kB, k8))], std::nullopt);
  EXPECT_EQ(array[index(Square(kC, k8))], std::nullopt);
  EXPECT_EQ(array[index(Square(kD, k8))], Piece(kQueen, kBlack));
  EXPECT_EQ(array[index(Square(kE, k8))], Piece(kKing, kBlack));
  EXPECT_EQ(array[index(Square(kF, k8))], std::nullopt);
  EXPECT_EQ(array[index(Square(kG, k8))], std::nullopt);
  EXPECT_EQ(array[index(Square(kH, k8))], Piece(kQueen, kWhite));

  // Rank 7: pppppppp
  for (auto file : {kA, kB, kC, kD, kE, kF, kG, kH}) {
    EXPECT_EQ(array[index(Square(file, k7))], Piece(kPawn, kBlack));
  }

  // Rank 6: 1nb1Pbnr
  EXPECT_EQ(array[index(Square(kA, k6))], std::nullopt);
  EXPECT_EQ(array[index(Square(kB, k6))], Piece(kKnight, kBlack));
  EXPECT_EQ(array[index(Square(kC, k6))], Piece(kBishop, kBlack));
  EXPECT_EQ(array[index(Square(kD, k6))], std::nullopt);
  EXPECT_EQ(array[index(Square(kE, k6))], Piece(kPawn, kWhite));
  EXPECT_EQ(array[index(Square(kF, k6))], Piece(kBishop, kBlack));
  EXPECT_EQ(array[index(Square(kG, k6))], Piece(kKnight, kBlack));
  EXPECT_EQ(array[index(Square(kH, k6))], Piece(kRook, kBlack));

  // Rank 5: 8 (empty)
  for (auto file : {kA, kB, kC, kD, kE, kF, kG, kH}) {
    EXPECT_EQ(array[index(Square(file, k5))], std::nullopt);
  }

  // Rank 4: R2Pp3
  EXPECT_EQ(array[index(Square(kA, k4))], Piece(kRook, kWhite));
  EXPECT_EQ(array[index(Square(kB, k4))], std::nullopt);
  EXPECT_EQ(array[index(Square(kC, k4))], std::nullopt);
  EXPECT_EQ(array[index(Square(kD, k4))], Piece(kPawn, kWhite));
  EXPECT_EQ(array[index(Square(kE, k4))], Piece(kPawn, kBlack));
  EXPECT_EQ(array[index(Square(kF, k4))], std::nullopt);
  EXPECT_EQ(array[index(Square(kG, k4))], std::nullopt);
  EXPECT_EQ(array[index(Square(kH, k4))], std::nullopt);

  // Rank 3 and 2: 8 (empty)
  for (auto rank : {k3, k2}) {
    for (auto file : {kA, kB, kC, kD, kE, kF, kG, kH}) {
      EXPECT_EQ(array[index(Square(file, rank))], std::nullopt);
    }
  }

  // Rank 1: 1NBQKBNR
  EXPECT_EQ(array[index(Square(kA, k1))], std::nullopt);
  EXPECT_EQ(array[index(Square(kB, k1))], Piece(kKnight, kWhite));
  EXPECT_EQ(array[index(Square(kC, k1))], Piece(kBishop, kWhite));
  EXPECT_EQ(array[index(Square(kD, k1))], Piece(kQueen, kWhite));
  EXPECT_EQ(array[index(Square(kE, k1))], Piece(kKing, kWhite));
  EXPECT_EQ(array[index(Square(kF, k1))], Piece(kBishop, kWhite));
  EXPECT_EQ(array[index(Square(kG, k1))], Piece(kKnight, kWhite));
  EXPECT_EQ(array[index(Square(kH, k1))], Piece(kRook, kWhite));
}

TEST(PiecePlacementTest, HashProducesFewCollisions) {
  // Adjust based on expectations for the test set.
  constexpr int max_collisions = 1;
  std::unordered_map<size_t, std::vector<chesskit::PiecePlacement>>
      hash_counter;

  std::ranges::for_each(GetValidInputs(), [&](const auto& fixture) {
    const auto& pp = fixture.piece_placement();
    auto hash = std::hash<chesskit::PiecePlacement>{}(pp);
    hash_counter[hash].push_back(pp);
    auto collisions = hash_counter[hash].size();
    EXPECT_LE(collisions, max_collisions)
        << std::format("pp={}, hash_counter={}", pp, hash_counter);
  });
}

TEST(PiecePlacementTest, HashProducesExpectedCollision) {
  auto lhs_pp =
      chesskit::parse<chesskit::PiecePlacement>("1RB1k3/8/8/8/8/8/8/4K3");
  auto rhs_pp =
      chesskit::parse<chesskit::PiecePlacement>("1rN1k3/8/8/8/8/8/8/4K3");
  ASSERT_TRUE(lhs_pp);
  ASSERT_TRUE(rhs_pp);
  EXPECT_NE(*lhs_pp, *rhs_pp);
  EXPECT_EQ(std::hash<chesskit::PiecePlacement>{}(*lhs_pp),
            std::hash<chesskit::PiecePlacement>{}(*rhs_pp))
      << std::format("lhs_pp:{} rhs_pp{}", *lhs_pp, *rhs_pp);
}

TEST(PiecePlacementTest, FormatProducesUniqueStrings) {
  using CollisionMap =
      std::unordered_map<std::string, std::vector<chesskit::PiecePlacement>>;
  CollisionMap fmt_collisions;
  CollisionMap ascii_fmt_collisions;
  CollisionMap lists_fmt_collisions;

  auto assert_unique_insertion = [](auto& map, const auto& key,
                                    const auto& val) {
    map[key].push_back(val);
    EXPECT_EQ(map[key].size(), 1) << std::format("pp={}, map={}", val, map);
  };

  std::ranges::for_each(GetValidInputs(), [&](const auto& fixture) {
    const auto& pp = fixture.piece_placement();
    assert_unique_insertion(fmt_collisions, std::format("{}", pp), pp);
    assert_unique_insertion(ascii_fmt_collisions, std::format("{:ascii}", pp),
                            pp);
    assert_unique_insertion(lists_fmt_collisions, std::format("{:lists}", pp),
                            pp);
  });
}
