#include <chesskit/castling_rights.h>
#include <chesskit/color.h>
#include <chesskit/file.h>
#include <chesskit/parse.h>
#include <chesskit/parse_error.h>
#include <chesskit/piece_placement.h>
#include <chesskit/position.h>
#include <chesskit/position_error.h>
#include <chesskit/rank.h>
#include <chesskit/square.h>
#include <gtest/gtest.h>
#include <yaml-cpp/yaml.h>

#include <algorithm>
#include <array>
#include <cstddef>
#include <expected>
#include <format>
#include <functional>
#include <magic_enum/magic_enum.hpp>
#include <optional>
#include <string>
#include <string_view>
#include <tuple>
#include <unordered_map>
#include <vector>

#include "test_helper.h"

namespace chesskit {
template <>
class Parser<Position::Params, const char*, parse_as::Default> {
 public:
  std::expected<ParseResult<Position::Params, const char*>, ParseError> parse(
      const char* begin, const char* end) {
    return internal::parsePositionParams(begin, end);
  }
};
}  // namespace chesskit

class ValidFixture {
 public:
  void set_input(std::string_view raw) {
    raw_ = raw;
    params_ = chesskit::parse<chesskit::Position::Params>(raw).value();
    position_ = chesskit::parse<chesskit::Position>(raw).value();
  }

  const std::string& raw() const { return raw_; }
  const chesskit::Position::Params& params() const { return params_; }
  const chesskit::Position& position() const { return position_; }

 private:
  std::string raw_;
  chesskit::Position::Params params_;
  chesskit::Position position_;
};

template <>
struct YAML::convert<ValidFixture> {
  static bool decode(const Node& node, ValidFixture& rhs) {
    rhs.set_input(node.as<std::string>());
    return true;
  }
};

class InvalidParamsFixture {
 public:
  void set_input(std::string_view raw) {
    raw_ = raw;
    params_ = chesskit::parse<chesskit::Position::Params>(raw).value();
  }
  void set_error(chesskit::PositionError error) { error_ = error; }

  const std::string& raw() const { return raw_; }
  const chesskit::Position::Params& params() const { return params_; }
  const chesskit::PositionError& error() const { return error_; }

 private:
  std::string raw_;
  chesskit::Position::Params params_;
  chesskit::PositionError error_;
};

template <>
struct YAML::convert<InvalidParamsFixture> {
  static bool decode(const Node& node, InvalidParamsFixture& rhs) {
    rhs.set_input(node[0].as<std::string>());

    auto error = magic_enum::enum_cast<chesskit::PositionError>(
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

  const std::string& raw() const { return raw_; }
  const chesskit::ParseError& error() const { return error_; }

 private:
  std::string raw_;
  chesskit::ParseError error_;
};

template <>
struct YAML::convert<InvalidFixture> {
  static bool decode(const Node& node, InvalidFixture& rhs) {
    rhs.set_input(node[0].as<std::string>());

    auto error =
        magic_enum::enum_cast<chesskit::ParseError>(node[1].as<std::string>());
    if (!error.has_value()) return false;
    rhs.set_error(*error);

    return true;
  }
};

auto GetConfig() { return YAML::LoadFile("data/position.yaml"); }

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

auto GetRepetitionInputs() {
  return GetConfig()["repetition_inputs"].as<std::vector<ValidFixture>>();
}

auto BuildRepetitionInputPairs() {
  return chesskit::testing::make_distinct_pairs(GetRepetitionInputs());
}

auto GetNoLegalEnPassantInputs() {
  return GetConfig()["no_legal_en_passant_inputs"]
      .as<std::vector<ValidFixture>>();
}

auto GetLegalEnPassantInputs() {
  return GetConfig()["legal_en_passant_inputs"].as<std::vector<ValidFixture>>();
}

auto GetInvalidParamsInputs() {
  return GetConfig()["invalid_params_inputs"]
      .as<std::vector<InvalidParamsFixture>>();
}

auto GetInvalidInputs() {
  return GetConfig()["invalid_inputs"].as<std::vector<InvalidFixture>>();
}

class ValidInputSuite : public ::testing::TestWithParam<ValidFixture> {};
INSTANTIATE_TEST_SUITE_P(PositionTest, ValidInputSuite,
                         ::testing::ValuesIn(GetValidInputs()));

class NoLegalEnPassantInputSuite : public ValidInputSuite {};
INSTANTIATE_TEST_SUITE_P(PositionTest, NoLegalEnPassantInputSuite,
                         ::testing::ValuesIn(GetNoLegalEnPassantInputs()));

class LegalEnPassantInputSuite : public ValidInputSuite {};
INSTANTIATE_TEST_SUITE_P(PositionTest, LegalEnPassantInputSuite,
                         ::testing::ValuesIn(GetLegalEnPassantInputs()));

class ValidInputPairSuite
    : public ::testing::TestWithParam<std::tuple<ValidFixture, ValidFixture>> {
};
INSTANTIATE_TEST_SUITE_P(PositionTest, ValidInputPairSuite,
                         ::testing::ValuesIn(BuildValidInputPairs()));

class RepetitionInputPairSuite : public ValidInputPairSuite {};
INSTANTIATE_TEST_SUITE_P(PositionTest, RepetitionInputPairSuite,
                         ::testing::ValuesIn(BuildRepetitionInputPairs()));

class InvalidParamsSuite
    : public ::testing::TestWithParam<InvalidParamsFixture> {};
INSTANTIATE_TEST_SUITE_P(PositionTest, InvalidParamsSuite,
                         ::testing::ValuesIn(GetInvalidParamsInputs()));

class InvalidInputSuite : public ::testing::TestWithParam<InvalidFixture> {};
INSTANTIATE_TEST_SUITE_P(PositionTest, InvalidInputSuite,
                         ::testing::ValuesIn(GetInvalidInputs()));

TEST_P(ValidInputSuite, ParsesSuccessfully) {
  const auto& fen = GetParam().raw();
  auto position = chesskit::parse<chesskit::Position>(fen);
  ASSERT_TRUE(position);
}

TEST_P(ValidInputSuite, ConstructsFromParamsSuccessfully) {
  const auto& params = GetParam().params();
  auto position = chesskit::Position::fromParams(params);
  ASSERT_TRUE(position);
  EXPECT_EQ(position->piecePlacement(), params.piecePlacement);
  EXPECT_EQ(position->activeColor(), params.activeColor);
  EXPECT_EQ(position->enPassantTargetSquare(), params.enPassantTargetSquare);
  EXPECT_EQ(position->halfmoveClock(), params.halfmoveClock);
  EXPECT_EQ(position->fullmoveNumber(), params.fullmoveNumber);
  EXPECT_EQ(position->castlingRights(), params.castlingRights);
}

TEST_P(ValidInputSuite, ComparesEqual) {
  auto lhs = GetParam().position();
  auto rhs = GetParam().position();
  EXPECT_EQ(lhs, lhs);
  EXPECT_EQ(lhs, rhs);
}

TEST_P(ValidInputSuite, ComparesRepetitionEqual) {
  auto lhs = GetParam().position();
  auto rhs = GetParam().position();
  EXPECT_TRUE(chesskit::RepetitionEqual{}(lhs, lhs));
  EXPECT_TRUE(chesskit::RepetitionEqual{}(lhs, rhs));
}

TEST_P(ValidInputSuite, RoundTripConversionIsSuccessful) {
  const auto& position = GetParam().position();
  EXPECT_EQ(position, chesskit::parse<chesskit::Position>(
                          std::format("{:fen}", position)));
}

TEST_P(ValidInputSuite, FormatProducesNonEmptyStrings) {
  auto position = GetParam().position();
  EXPECT_FALSE(std::format("{}", position).empty());
  EXPECT_FALSE(std::format("{:fen}", position).empty());
  EXPECT_FALSE(std::format("{:ascii}", position).empty());
  EXPECT_FALSE(std::format("{:lists}", position).empty());
  EXPECT_FALSE(std::format("{:rep}", position).empty());
}

TEST_P(NoLegalEnPassantInputSuite, ReturnsNullLegalEnPassant) {
  const auto& position = GetParam().position();
  EXPECT_NE(position.legalEnPassantTargetSquare(),
            position.enPassantTargetSquare());
  EXPECT_EQ(position.legalEnPassantTargetSquare(), std::nullopt);
}

TEST_P(LegalEnPassantInputSuite, ReturnsNonNullLegalEnPassant) {
  const auto& position = GetParam().position();
  EXPECT_EQ(position.legalEnPassantTargetSquare(),
            position.enPassantTargetSquare());
  EXPECT_NE(position.legalEnPassantTargetSquare(), std::nullopt);
}

TEST_P(NoLegalEnPassantInputSuite, IsEquivalentWhenEnPassantIsCleared) {
  const auto& params = GetParam().params();

  auto with_en_passant = chesskit::Position::fromParams(params);
  auto new_params = params;
  new_params.enPassantTargetSquare = std::nullopt;
  auto without_en_passant = chesskit::Position::fromParams(new_params);

  ASSERT_TRUE(with_en_passant);
  ASSERT_TRUE(without_en_passant);
  EXPECT_NE(with_en_passant->enPassantTargetSquare(),
            without_en_passant->enPassantTargetSquare());

  EXPECT_EQ(with_en_passant->legalEnPassantTargetSquare(),
            without_en_passant->legalEnPassantTargetSquare());
  EXPECT_TRUE(
      chesskit::RepetitionEqual{}(*with_en_passant, *without_en_passant));
  EXPECT_EQ(chesskit::RepetitionHash{}(*with_en_passant),
            chesskit::RepetitionHash{}(*without_en_passant));
  EXPECT_EQ(std::format("{:rep}", *with_en_passant),
            std::format("{:rep}", *without_en_passant));
}

TEST_P(LegalEnPassantInputSuite, IsDifferentWhenEnPassantIsCleared) {
  const auto& params = GetParam().params();

  auto with_en_passant = chesskit::Position::fromParams(params);
  auto new_params = params;
  new_params.enPassantTargetSquare = std::nullopt;
  auto without_en_passant = chesskit::Position::fromParams(new_params);

  ASSERT_TRUE(with_en_passant);
  ASSERT_TRUE(without_en_passant);
  EXPECT_NE(with_en_passant->enPassantTargetSquare(),
            without_en_passant->enPassantTargetSquare());

  EXPECT_NE(with_en_passant->legalEnPassantTargetSquare(),
            without_en_passant->legalEnPassantTargetSquare());
  EXPECT_FALSE(
      chesskit::RepetitionEqual{}(*with_en_passant, *without_en_passant));
  // These hashes may collide but are likely different
  EXPECT_NE(chesskit::RepetitionHash{}(*with_en_passant),
            chesskit::RepetitionHash{}(*without_en_passant));
  EXPECT_NE(std::format("{:rep}", *with_en_passant),
            std::format("{:rep}", *without_en_passant));
}

TEST_P(ValidInputPairSuite, ComparesUnequal) {
  const auto& [lhs, rhs] = GetParam();
  EXPECT_NE(lhs.position(), rhs.position());
}

TEST_P(RepetitionInputPairSuite, ComparesRepetitionUnequal) {
  const auto& [lhs, rhs] = GetParam();
  EXPECT_FALSE(chesskit::RepetitionEqual{}(lhs.position(), rhs.position()));
}

TEST_P(InvalidParamsSuite, FromParamsReturnsCorrectError) {
  const auto& fixture = GetParam();
  EXPECT_EQ(chesskit::Position::fromParams(fixture.params()).error(),
            fixture.error());
}

TEST_P(InvalidParamsSuite, ParseReturnsCorrectError) {
  const auto& fixture = GetParam();
  EXPECT_EQ(chesskit::parse<chesskit::Position>(fixture.raw()).error(),
            chesskit::ParseError::kInvalidPosition);
}

TEST_P(InvalidInputSuite, ParseHandlesInvalidInputCorrectly) {
  const auto& fixture = GetParam();
  EXPECT_EQ(chesskit::parse<chesskit::Position>(fixture.raw()).error(),
            fixture.error());
}

TEST(PositionParamsTest, DefaultConstructionCreatesDefaultStartingPosition) {
  chesskit::Position::Params params;
  auto expected = GetDefault().params();

  EXPECT_EQ(params.piecePlacement, expected.piecePlacement);
  EXPECT_EQ(params.activeColor, expected.activeColor);
  EXPECT_EQ(params.enPassantTargetSquare, expected.enPassantTargetSquare);
  EXPECT_EQ(params.halfmoveClock, expected.halfmoveClock);
  EXPECT_EQ(params.fullmoveNumber, expected.fullmoveNumber);
  EXPECT_EQ(params.castlingRights, expected.castlingRights);
}

TEST(PositionTest, DefaultConstructionCreatesDefaultStartingPosition) {
  chesskit::Position position;
  auto expected = GetDefault().params();

  EXPECT_EQ(position.piecePlacement(), expected.piecePlacement);
  EXPECT_EQ(position.activeColor(), expected.activeColor);
  EXPECT_EQ(position.enPassantTargetSquare(), expected.enPassantTargetSquare);
  EXPECT_EQ(position.legalEnPassantTargetSquare(), std::nullopt);
  EXPECT_EQ(position.halfmoveClock(), expected.halfmoveClock);
  EXPECT_EQ(position.fullmoveNumber(), expected.fullmoveNumber);
  EXPECT_EQ(position.castlingRights(), expected.castlingRights);
}

TEST(PositionTest, HashProducesFewCollisions) {
  // Adjust based on expectations for the test set.
  constexpr int max_collisions = 1;
  std::unordered_map<size_t, std::vector<chesskit::Position>> hash_counter;

  std::ranges::for_each(GetValidInputs(), [&](const auto& fixture) {
    const auto& position = fixture.position();
    auto hash = std::hash<chesskit::Position>{}(position);
    hash_counter[hash].push_back(position);
    auto collisions = hash_counter[hash].size();
    EXPECT_LE(collisions, max_collisions)
        << std::format("position={}, hash_counter={}", position, hash_counter);
  });
}

TEST(PositionRepetitionTest, HashProducesFewCollisions) {
  // Adjust based on expectations for the test set.
  constexpr int max_collisions = 1;
  std::unordered_map<size_t, std::vector<chesskit::Position>> hash_counter;

  std::ranges::for_each(GetRepetitionInputs(), [&](const auto& fixture) {
    const auto& position = fixture.position();
    auto hash = chesskit::RepetitionHash{}(position);
    hash_counter[hash].push_back(position);
    auto collisions = hash_counter[hash].size();
    EXPECT_LE(collisions, max_collisions)
        << std::format("position={}, hash_counter={}", position, hash_counter);
  });
}

TEST(PositionTest, FormatComplexInputCorrectly) {
  auto complex = GetComplexInput();
  const auto& position = complex.position();
  EXPECT_EQ(std::format("{}", position), complex.raw());
  EXPECT_EQ(std::format("{:fen}", position), complex.raw());
  EXPECT_EQ(std::format("{:ascii}", position),
            "r..qk..Q\n"
            "pppppppp\n"
            ".nb.Pbnr\n"
            "........\n"
            "R..Pp...\n"
            "........\n"
            "........\n"
            ".NBQKBNR\n"
            "Active color: black\n"
            "Castling availability: Kq\n"
            "En passant target square: d3\n"
            "Halfmove clock: 4294967295\n"
            "Fullmove number: 4294967295");
  EXPECT_EQ(
      std::format("{:lists}", position),
      "{ white king: [e1], white bishops: [f1, c1], white knights: [g1, "
      "b1], white rooks: [h1, a4], white pawns: [d4, e6], white queens: [d1, "
      "h8], black bishops: [f6, c6], black knights: [g6, b6], black "
      "pawns: [h7, g7, f7, e7, d7, e4, c7, b7, a7], black king: [e8], "
      "black queen: [d8], black rooks: [h6, a8] }\n"
      "Active color: black\n"
      "Castling availability: Kq\n"
      "En passant target square: d3\n"
      "Halfmove clock: 4294967295\n"
      "Fullmove number: 4294967295");
  EXPECT_EQ(std::format("{:rep}", position),
            "r2qk2Q/pppppppp/1nb1Pbnr/8/R2Pp3/8/8/1NBQKBNR b Kq -");
}

TEST(PositionTest, ParseComplexInputCorrectly) {
  using enum chesskit::Color;
  using enum chesskit::Rank;
  using enum chesskit::File;
  using chesskit::Square, chesskit::CastlingRights;

  auto expected_pp = chesskit::parse<chesskit::PiecePlacement>(
      "r2qk2Q/pppppppp/1nb1Pbnr/8/R2Pp3/8/8/1NBQKBNR");
  ASSERT_TRUE(expected_pp);

  auto parsed_position =
      chesskit::parse<chesskit::Position>(GetComplexInput().raw());
  ASSERT_TRUE(parsed_position);

  EXPECT_EQ(parsed_position->piecePlacement(), expected_pp);
  EXPECT_EQ(parsed_position->activeColor(), kBlack);
  EXPECT_EQ(parsed_position->enPassantTargetSquare(), Square(kD, k3));
  EXPECT_EQ(parsed_position->halfmoveClock(), 4294967295);
  EXPECT_EQ(parsed_position->fullmoveNumber(), 4294967295);
  EXPECT_EQ(parsed_position->castlingRights(), CastlingRights(0b1001));
}

TEST(PositionTest, FormatProducesUniqueStrings) {
  using CollisionMap =
      std::unordered_map<std::string, std::vector<chesskit::Position>>;
  CollisionMap fmt_collisions;
  CollisionMap ascii_fmt_collisions;
  CollisionMap lists_fmt_collisions;

  auto assert_unique_insertion = [](auto& map, const auto& key,
                                    const auto& val) {
    map[key].push_back(val);
    EXPECT_EQ(map[key].size(), 1)
        << std::format("position={}, map={}", val, map);
  };

  std::ranges::for_each(GetValidInputs(), [&](const auto& fixture) {
    const auto& position = fixture.position();
    assert_unique_insertion(fmt_collisions, std::format("{}", position),
                            position);
    assert_unique_insertion(ascii_fmt_collisions,
                            std::format("{:ascii}", position), position);
    assert_unique_insertion(lists_fmt_collisions,
                            std::format("{:lists}", position), position);
  });
}

TEST(PositionRepetitionTest, FormatProducesUniqueStrings) {
  using CollisionMap =
      std::unordered_map<std::string, std::vector<chesskit::Position>>;
  CollisionMap rep_fmt_collisions;

  auto assert_unique_insertion = [](auto& map, const auto& key,
                                    const auto& val) {
    map[key].push_back(val);
    EXPECT_EQ(map[key].size(), 1)
        << std::format("position={}, map={}", val, map);
  };

  std::ranges::for_each(GetRepetitionInputs(), [&](const auto& fixture) {
    const auto& position = fixture.position();
    assert_unique_insertion(rep_fmt_collisions, std::format("{:rep}", position),
                            position);
  });
}

TEST(PositionRepetitionTest, ConsiderPiecePlacement) {
  auto new_pp =
      chesskit::parse<chesskit::PiecePlacement>("r3k2r/8/8/8/8/8/8/R3K2R");
  ASSERT_TRUE(new_pp);

  chesskit::Position default_position;
  auto diff_piece_placement = chesskit::Position::fromParams({
      .piecePlacement = new_pp.value(),
  });

  ASSERT_TRUE(diff_piece_placement);
  EXPECT_FALSE(
      chesskit::RepetitionEqual{}(default_position, *diff_piece_placement));
  // These hashes may collide but are likely different
  EXPECT_NE(chesskit::RepetitionHash{}(default_position),
            chesskit::RepetitionHash{}(*diff_piece_placement));
  EXPECT_NE(std::format("{:rep}", default_position),
            std::format("{:rep}", *diff_piece_placement));
}

TEST(PositionRepetitionTest, ConsiderColor) {
  chesskit::Position default_position;
  auto diff_active_color = chesskit::Position::fromParams({
      .activeColor = chesskit::Color::kBlack,
  });

  ASSERT_TRUE(diff_active_color);
  EXPECT_FALSE(
      chesskit::RepetitionEqual{}(default_position, *diff_active_color));
  // These hashes may collide but are likely different
  EXPECT_NE(chesskit::RepetitionHash{}(default_position),
            chesskit::RepetitionHash{}(*diff_active_color));
  EXPECT_NE(std::format("{:rep}", default_position),
            std::format("{:rep}", *diff_active_color));
}

TEST(PositionRepetitionTest, ConsiderCastlingRights) {
  chesskit::Position default_position;
  auto diff_castling_rights = chesskit::Position::fromParams({
      .castlingRights = chesskit::CastlingRights(0),
  });

  ASSERT_TRUE(diff_castling_rights);
  EXPECT_FALSE(
      chesskit::RepetitionEqual{}(default_position, *diff_castling_rights));
  // These hashes may collide but are likely different
  EXPECT_NE(chesskit::RepetitionHash{}(default_position),
            chesskit::RepetitionHash{}(*diff_castling_rights));
  EXPECT_NE(std::format("{:rep}", default_position),
            std::format("{:rep}", *diff_castling_rights));
}

TEST(PositionRepetitionTest, IgnoresMoveCounters) {
  chesskit::Position default_position;
  auto diff_move_counters = chesskit::Position::fromParams({
      .halfmoveClock = 10,
      .fullmoveNumber = 10,
  });

  ASSERT_TRUE(diff_move_counters);
  EXPECT_TRUE(
      chesskit::RepetitionEqual{}(default_position, *diff_move_counters));
  EXPECT_EQ(chesskit::RepetitionHash{}(default_position),
            chesskit::RepetitionHash{}(*diff_move_counters));
  EXPECT_EQ(std::format("{:rep}", default_position),
            std::format("{:rep}", *diff_move_counters));
}
