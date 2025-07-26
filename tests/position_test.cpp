#include <chesscxx/castling_rights.h>
#include <chesscxx/color.h>
#include <chesscxx/file.h>
#include <chesscxx/parse.h>
#include <chesscxx/parse_error.h>
#include <chesscxx/piece_placement.h>
#include <chesscxx/position.h>
#include <chesscxx/position_error.h>
#include <chesscxx/rank.h>
#include <chesscxx/square.h>
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

namespace chesscxx {
template <>
class Parser<Position::Params, const char*, parse_as::Default> {
 public:
  static auto parse(const char* begin, const char* end)
      -> std::expected<ParseResult<Position::Params, const char*>, ParseError> {
    return internal::parsePositionParams(begin, end);
  }
};
}  // namespace chesscxx

class ValidFixture {
 public:
  void set_input(std::string_view raw) {
    raw_ = raw;
    params_ = chesscxx::parse<chesscxx::Position::Params>(raw).value();
    position_ = chesscxx::parse<chesscxx::Position>(raw).value();
  }

  auto raw() const -> const std::string& { return raw_; }
  auto params() const -> const chesscxx::Position::Params& { return params_; }
  auto position() const -> const chesscxx::Position& { return position_; }

 private:
  std::string raw_;
  chesscxx::Position::Params params_;
  chesscxx::Position position_;
};

template <>
struct YAML::convert<ValidFixture> {
  static auto decode(const Node& node, ValidFixture& rhs) -> bool {
    rhs.set_input(node.as<std::string>());
    return true;
  }
};

class InvalidParamsFixture {
 public:
  void set_input(std::string_view raw) {
    raw_ = raw;
    params_ = chesscxx::parse<chesscxx::Position::Params>(raw).value();
  }
  void set_error(chesscxx::PositionError error) { error_ = error; }

  auto raw() const -> const std::string& { return raw_; }
  auto params() const -> const chesscxx::Position::Params& { return params_; }
  auto error() const -> const chesscxx::PositionError& { return error_; }

 private:
  std::string raw_;
  chesscxx::Position::Params params_;
  chesscxx::PositionError error_{};
};

template <>
struct YAML::convert<InvalidParamsFixture> {
  static auto decode(const Node& node, InvalidParamsFixture& rhs) -> bool {
    rhs.set_input(node[0].as<std::string>());

    auto error = magic_enum::enum_cast<chesscxx::PositionError>(
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
auto GetConfig() { return YAML::LoadFile("data/position.yaml"); }

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

auto GetRepetitionInputs() {
  return GetConfig()["repetition_inputs"].as<std::vector<ValidFixture>>();
}

auto BuildRepetitionInputPairs() {
  return chesscxx::testing::make_distinct_pairs(GetRepetitionInputs());
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
}  // namespace

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
  auto position = chesscxx::parse<chesscxx::Position>(fen);
  ASSERT_TRUE(position);
}

TEST_P(ValidInputSuite, ConstructsFromParamsSuccessfully) {
  const auto& params = GetParam().params();
  auto position = chesscxx::Position::fromParams(params);
  ASSERT_TRUE(position);
  EXPECT_EQ(position->piecePlacement(), params.piece_placement);
  EXPECT_EQ(position->activeColor(), params.active_color);
  EXPECT_EQ(position->enPassantTargetSquare(), params.en_passant_target_square);
  EXPECT_EQ(position->halfmoveClock(), params.halfmove_clock);
  EXPECT_EQ(position->fullmoveNumber(), params.fullmove_number);
  EXPECT_EQ(position->castlingRights(), params.castling_rights);
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
  EXPECT_TRUE(chesscxx::RepetitionEqual{}(lhs, lhs));
  EXPECT_TRUE(chesscxx::RepetitionEqual{}(lhs, rhs));
}

TEST_P(ValidInputSuite, RoundTripConversionIsSuccessful) {
  const auto& position = GetParam().position();
  EXPECT_EQ(position, chesscxx::parse<chesscxx::Position>(
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

  auto with_en_passant = chesscxx::Position::fromParams(params);
  auto new_params = params;
  new_params.en_passant_target_square = std::nullopt;
  auto without_en_passant = chesscxx::Position::fromParams(new_params);

  ASSERT_TRUE(with_en_passant);
  ASSERT_TRUE(without_en_passant);
  EXPECT_NE(with_en_passant->enPassantTargetSquare(),
            without_en_passant->enPassantTargetSquare());

  EXPECT_EQ(with_en_passant->legalEnPassantTargetSquare(),
            without_en_passant->legalEnPassantTargetSquare());
  EXPECT_TRUE(
      chesscxx::RepetitionEqual{}(*with_en_passant, *without_en_passant));
  EXPECT_EQ(chesscxx::RepetitionHash{}(*with_en_passant),
            chesscxx::RepetitionHash{}(*without_en_passant));
  EXPECT_EQ(std::format("{:rep}", *with_en_passant),
            std::format("{:rep}", *without_en_passant));
}

TEST_P(LegalEnPassantInputSuite, IsDifferentWhenEnPassantIsCleared) {
  const auto& params = GetParam().params();

  auto with_en_passant = chesscxx::Position::fromParams(params);
  auto new_params = params;
  new_params.en_passant_target_square = std::nullopt;
  auto without_en_passant = chesscxx::Position::fromParams(new_params);

  ASSERT_TRUE(with_en_passant);
  ASSERT_TRUE(without_en_passant);
  EXPECT_NE(with_en_passant->enPassantTargetSquare(),
            without_en_passant->enPassantTargetSquare());

  EXPECT_NE(with_en_passant->legalEnPassantTargetSquare(),
            without_en_passant->legalEnPassantTargetSquare());
  EXPECT_FALSE(
      chesscxx::RepetitionEqual{}(*with_en_passant, *without_en_passant));
  // These hashes may collide but are likely different
  EXPECT_NE(chesscxx::RepetitionHash{}(*with_en_passant),
            chesscxx::RepetitionHash{}(*without_en_passant));
  EXPECT_NE(std::format("{:rep}", *with_en_passant),
            std::format("{:rep}", *without_en_passant));
}

TEST_P(ValidInputPairSuite, ComparesUnequal) {
  const auto& [lhs, rhs] = GetParam();
  EXPECT_NE(lhs.position(), rhs.position());
}

TEST_P(RepetitionInputPairSuite, ComparesRepetitionUnequal) {
  const auto& [lhs, rhs] = GetParam();
  EXPECT_FALSE(chesscxx::RepetitionEqual{}(lhs.position(), rhs.position()));
}

TEST_P(InvalidParamsSuite, FromParamsReturnsCorrectError) {
  const auto& fixture = GetParam();
  EXPECT_EQ(chesscxx::Position::fromParams(fixture.params()).error(),
            fixture.error());
}

TEST_P(InvalidParamsSuite, ParseReturnsCorrectError) {
  const auto& fixture = GetParam();
  EXPECT_EQ(chesscxx::parse<chesscxx::Position>(fixture.raw()).error(),
            chesscxx::ParseError::kInvalidPosition);
}

TEST_P(InvalidInputSuite, ParseHandlesInvalidInputCorrectly) {
  const auto& fixture = GetParam();
  EXPECT_EQ(chesscxx::parse<chesscxx::Position>(fixture.raw()).error(),
            fixture.error());
}

TEST(PositionParamsTest, DefaultConstructionCreatesDefaultStartingPosition) {
  chesscxx::Position::Params const params;
  auto expected = GetDefault().params();

  EXPECT_EQ(params.piece_placement, expected.piece_placement);
  EXPECT_EQ(params.active_color, expected.active_color);
  EXPECT_EQ(params.en_passant_target_square, expected.en_passant_target_square);
  EXPECT_EQ(params.halfmove_clock, expected.halfmove_clock);
  EXPECT_EQ(params.fullmove_number, expected.fullmove_number);
  EXPECT_EQ(params.castling_rights, expected.castling_rights);
}

TEST(PositionTest, DefaultConstructionCreatesDefaultStartingPosition) {
  chesscxx::Position const position;
  auto expected = GetDefault().params();

  EXPECT_EQ(position.piecePlacement(), expected.piece_placement);
  EXPECT_EQ(position.activeColor(), expected.active_color);
  EXPECT_EQ(position.enPassantTargetSquare(),
            expected.en_passant_target_square);
  EXPECT_EQ(position.legalEnPassantTargetSquare(), std::nullopt);
  EXPECT_EQ(position.halfmoveClock(), expected.halfmove_clock);
  EXPECT_EQ(position.fullmoveNumber(), expected.fullmove_number);
  EXPECT_EQ(position.castlingRights(), expected.castling_rights);
}

TEST(PositionTest, HashProducesFewCollisions) {
  // Adjust based on expectations for the test set.
  constexpr int kMaxCollisions = 1;
  std::unordered_map<std::size_t, std::vector<chesscxx::Position>> hash_counter;

  std::ranges::for_each(GetValidInputs(), [&](const auto& fixture) {
    const auto& position = fixture.position();
    auto hash = std::hash<chesscxx::Position>{}(position);
    hash_counter[hash].push_back(position);
    auto collisions = hash_counter[hash].size();
    EXPECT_LE(collisions, kMaxCollisions)
        << std::format("position={}, hash_counter={}", position, hash_counter);
  });
}

TEST(PositionRepetitionTest, HashProducesFewCollisions) {
  // Adjust based on expectations for the test set.
  constexpr int kMaxCollisions = 1;
  std::unordered_map<std::size_t, std::vector<chesscxx::Position>> hash_counter;

  std::ranges::for_each(GetRepetitionInputs(), [&](const auto& fixture) {
    const auto& position = fixture.position();
    auto hash = chesscxx::RepetitionHash{}(position);
    hash_counter[hash].push_back(position);
    auto collisions = hash_counter[hash].size();
    EXPECT_LE(collisions, kMaxCollisions)
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
  using enum chesscxx::Color;
  using enum chesscxx::Rank;
  using enum chesscxx::File;
  using chesscxx::Square, chesscxx::CastlingRights;

  auto expected_pp = chesscxx::parse<chesscxx::PiecePlacement>(
      "r2qk2Q/pppppppp/1nb1Pbnr/8/R2Pp3/8/8/1NBQKBNR");
  ASSERT_TRUE(expected_pp);

  auto parsed_position =
      chesscxx::parse<chesscxx::Position>(GetComplexInput().raw());
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
      std::unordered_map<std::string, std::vector<chesscxx::Position>>;
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
      std::unordered_map<std::string, std::vector<chesscxx::Position>>;
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
      chesscxx::parse<chesscxx::PiecePlacement>("r3k2r/8/8/8/8/8/8/R3K2R");
  ASSERT_TRUE(new_pp);

  chesscxx::Position default_position;
  auto diff_piece_placement = chesscxx::Position::fromParams({
      .piece_placement = new_pp.value(),
  });

  ASSERT_TRUE(diff_piece_placement);
  EXPECT_FALSE(
      chesscxx::RepetitionEqual{}(default_position, *diff_piece_placement));
  // These hashes may collide but are likely different
  EXPECT_NE(chesscxx::RepetitionHash{}(default_position),
            chesscxx::RepetitionHash{}(*diff_piece_placement));
  EXPECT_NE(std::format("{:rep}", default_position),
            std::format("{:rep}", *diff_piece_placement));
}

TEST(PositionRepetitionTest, ConsiderColor) {
  chesscxx::Position default_position;
  auto diff_active_color = chesscxx::Position::fromParams({
      .active_color = chesscxx::Color::kBlack,
  });

  ASSERT_TRUE(diff_active_color);
  EXPECT_FALSE(
      chesscxx::RepetitionEqual{}(default_position, *diff_active_color));
  // These hashes may collide but are likely different
  EXPECT_NE(chesscxx::RepetitionHash{}(default_position),
            chesscxx::RepetitionHash{}(*diff_active_color));
  EXPECT_NE(std::format("{:rep}", default_position),
            std::format("{:rep}", *diff_active_color));
}

TEST(PositionRepetitionTest, ConsiderCastlingRights) {
  chesscxx::Position default_position;
  auto diff_castling_rights = chesscxx::Position::fromParams({
      .castling_rights = chesscxx::CastlingRights(0),
  });

  ASSERT_TRUE(diff_castling_rights);
  EXPECT_FALSE(
      chesscxx::RepetitionEqual{}(default_position, *diff_castling_rights));
  // These hashes may collide but are likely different
  EXPECT_NE(chesscxx::RepetitionHash{}(default_position),
            chesscxx::RepetitionHash{}(*diff_castling_rights));
  EXPECT_NE(std::format("{:rep}", default_position),
            std::format("{:rep}", *diff_castling_rights));
}

TEST(PositionRepetitionTest, IgnoresMoveCounters) {
  chesscxx::Position default_position;
  auto diff_move_counters = chesscxx::Position::fromParams({
      .halfmove_clock = 2,
      .fullmove_number = 2,
  });

  ASSERT_TRUE(diff_move_counters);
  EXPECT_TRUE(
      chesscxx::RepetitionEqual{}(default_position, *diff_move_counters));
  EXPECT_EQ(chesscxx::RepetitionHash{}(default_position),
            chesscxx::RepetitionHash{}(*diff_move_counters));
  EXPECT_EQ(std::format("{:rep}", default_position),
            std::format("{:rep}", *diff_move_counters));
}
