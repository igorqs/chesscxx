#include <chesscxx/draw_reason.h>
#include <chesscxx/game.h>
#include <chesscxx/game_result.h>
#include <chesscxx/move_error.h>
#include <chesscxx/parse.h>
#include <chesscxx/parse_error.h>
#include <chesscxx/position.h>
#include <chesscxx/san_move.h>
#include <chesscxx/uci_move.h>
#include <gtest/gtest.h>
#include <yaml-cpp/yaml.h>

#include <algorithm>
#include <cstddef>
#include <format>
#include <magic_enum/magic_enum.hpp>
#include <optional>
#include <string>
#include <string_view>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "test_helper.h"  // IWYU pragma: keep

class GameEqualityFixture {
 public:
  void add_game(const chesscxx::Game& game) { games_.push_back(game); }
  [[nodiscard]] auto games() const -> const std::vector<chesscxx::Game>& {
    return games_;
  }

 private:
  std::vector<chesscxx::Game> games_;
};

template <>
struct YAML::convert<GameEqualityFixture> {
  static auto decode(const Node& node, GameEqualityFixture& rhs) -> bool {
    for (const auto& game : node) {
      rhs.add_game(
          chesscxx::parse<chesscxx::Game>(game.as<std::string>()).value());
    }

    return true;
  }
};

class PositionFixture {
 public:
  void set_input(std::string_view raw) {
    raw_ = raw;
    position_ = chesscxx::parse<chesscxx::Position>(raw).value();
  }

  auto raw() const -> const std::string& { return raw_; }
  auto position() const -> const chesscxx::Position& { return position_; }

 private:
  std::string raw_;
  chesscxx::Position::Params params_;
  chesscxx::Position position_;
};

template <>
struct YAML::convert<PositionFixture> {
  static auto decode(const Node& node, PositionFixture& rhs) -> bool {
    rhs.set_input(node.as<std::string>());
    return true;
  }
};

class OutcomeFixture {
 public:
  void set_input(std::string_view raw) {
    raw_ = raw;
    ASSERT_TRUE(chesscxx::parse<chesscxx::Game>(raw)) << raw;
    game_ = chesscxx::parse<chesscxx::Game>(raw).value();
  }
  void set_result(std::optional<chesscxx::GameResult> result) {
    result_ = result;
  }
  void set_draw_reason(std::optional<chesscxx::DrawReason> draw_reason) {
    draw_reason_ = draw_reason;
  }

  auto raw() const -> const std::string& { return raw_; }
  auto game() const -> const chesscxx::Game& { return game_; }
  auto result() const -> const std::optional<chesscxx::GameResult>& {
    return result_;
  }
  auto draw_reason() const -> const std::optional<chesscxx::DrawReason>& {
    return draw_reason_;
  }

 private:
  std::string raw_;
  chesscxx::Game game_;
  std::optional<chesscxx::GameResult> result_;
  std::optional<chesscxx::DrawReason> draw_reason_;
};

template <>
struct YAML::convert<OutcomeFixture> {
  static auto decode(const Node& node, OutcomeFixture& rhs) -> bool {
    rhs.set_input(node[0].as<std::string>());

    if (!node[1].IsNull()) {
      rhs.set_result(magic_enum::enum_cast<chesscxx::GameResult>(
          node[1].as<std::string>()));
    }

    if (!node[2].IsNull()) {
      rhs.set_draw_reason(magic_enum::enum_cast<chesscxx::DrawReason>(
          node[2].as<std::string>()));
    }

    return true;
  }
};

class MoveFixture {
 public:
  void set_input(std::string_view raw) {
    raw_ = raw;
    ASSERT_TRUE(chesscxx::parse<chesscxx::Game>(raw)) << std::format(
        "{} {}", raw, chesscxx::parse<chesscxx::Game>(raw).error());
    game_ = chesscxx::parse<chesscxx::Game>(raw).value();
  }
  void add_san_move(chesscxx::SanMove san_move) {
    san_moves_.push_back(san_move);
  }
  void add_uci_move(chesscxx::UciMove uci_move) {
    uci_moves_.push_back(uci_move);
  }

  auto raw() const -> const std::string& { return raw_; }
  auto game() const -> const chesscxx::Game& { return game_; }
  auto san_moves() const -> const std::vector<chesscxx::SanMove>& {
    return san_moves_;
  }
  auto uci_moves() const -> const std::vector<chesscxx::UciMove>& {
    return uci_moves_;
  }

 private:
  std::string raw_;
  chesscxx::Game game_;
  std::vector<chesscxx::SanMove> san_moves_;
  std::vector<chesscxx::UciMove> uci_moves_;
};

template <>
struct YAML::convert<MoveFixture> {
  static auto decode(const Node& node, MoveFixture& rhs) -> bool {
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

class RepetitionTrackerFixture {
 public:
  void set_input(std::string_view raw) {
    raw_ = raw;
    ASSERT_TRUE(chesscxx::parse<chesscxx::Game>(raw)) << std::format(
        "{} {}", raw, chesscxx::parse<chesscxx::Game>(raw).error());
    game_ = chesscxx::parse<chesscxx::Game>(raw).value();
  }
  void add_repetition(const chesscxx::Position& position, int counter,
                      int undo_counter) {
    repetitions_.emplace_back(position, counter, undo_counter);
  }

  auto raw() const -> const std::string& { return raw_; }
  auto game() const -> const chesscxx::Game& { return game_; }
  auto repetitions()
      -> const std::vector<std::tuple<chesscxx::Position, int, int>>& {
    return repetitions_;
  }

 private:
  std::string raw_;
  chesscxx::Game game_;
  std::vector<std::tuple<chesscxx::Position, int, int>> repetitions_;
};

template <>
struct YAML::convert<RepetitionTrackerFixture> {
  static auto decode(const Node& node, RepetitionTrackerFixture& rhs) -> bool {
    rhs.set_input(node[0].as<std::string>());

    for (const auto& repetition : node[1]) {
      auto position =
          chesscxx::parse<chesscxx::Position>(repetition[0].as<std::string>())
              .value();
      auto quantity = repetition[1].as<int>();
      auto undo_quantity = repetition[2].as<int>();
      rhs.add_repetition(position, quantity, undo_quantity);
    }

    return true;
  }
};

class SanMoveErrorFixture {
 public:
  void set_input(std::string_view raw) {
    raw_ = raw;
    ASSERT_TRUE(chesscxx::parse<chesscxx::Game>(raw, chesscxx::parse_as::Fen{}))
        << std::format(
               "{} {}", raw,
               chesscxx::parse<chesscxx::Game>(raw, chesscxx::parse_as::Fen{})
                   .error());
    game_ =
        chesscxx::parse<chesscxx::Game>(raw, chesscxx::parse_as::Fen{}).value();
  }
  void set_move(chesscxx::SanMove move) { move_ = move; }
  void set_error(chesscxx::MoveError error) { error_ = error; }

  auto raw() const -> const std::string& { return raw_; }
  auto game() const -> const chesscxx::Game& { return game_; }
  auto move() const -> const chesscxx::SanMove& { return move_; }
  auto error() const -> const chesscxx::MoveError& { return error_; }

 private:
  std::string raw_;
  chesscxx::Game game_;
  chesscxx::SanMove move_;
  chesscxx::MoveError error_{};
};

template <>
struct YAML::convert<SanMoveErrorFixture> {
  static auto decode(const Node& node, SanMoveErrorFixture& rhs) -> bool {
    rhs.set_input(node[0].as<std::string>());

    rhs.set_move(
        chesscxx::parse<chesscxx::SanMove>(node[1].as<std::string>()).value());

    auto error =
        magic_enum::enum_cast<chesscxx::MoveError>(node[2].as<std::string>());
    if (!error.has_value()) return false;
    rhs.set_error(*error);

    return true;
  }
};

class UciMoveErrorFixture {
 public:
  void set_input(std::string_view raw) {
    raw_ = raw;
    ASSERT_TRUE(chesscxx::parse<chesscxx::Game>(raw, chesscxx::parse_as::Fen{}))
        << std::format(
               "{} {}", raw,
               chesscxx::parse<chesscxx::Game>(raw, chesscxx::parse_as::Fen{})
                   .error());
    game_ =
        chesscxx::parse<chesscxx::Game>(raw, chesscxx::parse_as::Fen{}).value();
  }
  void set_move(chesscxx::UciMove move) { move_ = move; }
  void set_error(chesscxx::MoveError error) { error_ = error; }

  auto raw() const -> const std::string& { return raw_; }
  auto game() const -> const chesscxx::Game& { return game_; }
  auto move() const -> const chesscxx::UciMove& { return move_; }
  auto error() const -> const chesscxx::MoveError& { return error_; }

 private:
  std::string raw_;
  chesscxx::Game game_;
  chesscxx::UciMove move_;
  chesscxx::MoveError error_{};
};

template <>
struct YAML::convert<UciMoveErrorFixture> {
  static auto decode(const Node& node, UciMoveErrorFixture& rhs) -> bool {
    rhs.set_input(node[0].as<std::string>());

    rhs.set_move(
        chesscxx::parse<chesscxx::UciMove>(node[1].as<std::string>()).value());

    auto error =
        magic_enum::enum_cast<chesscxx::MoveError>(node[2].as<std::string>());
    if (!error.has_value()) return false;
    rhs.set_error(*error);

    return true;
  }
};

class ValidMoveFixture {
 public:
  void set_input(std::string_view raw) {
    raw_ = raw;
    ASSERT_TRUE(chesscxx::parse<chesscxx::Game>(raw, chesscxx::parse_as::Fen{}))
        << std::format(
               "{} {}", raw,
               chesscxx::parse<chesscxx::Game>(raw, chesscxx::parse_as::Fen{})
                   .error());
    game_ =
        chesscxx::parse<chesscxx::Game>(raw, chesscxx::parse_as::Fen{}).value();
  }
  void set_uci_move(chesscxx::UciMove move) { uci_move_ = move; }
  void set_san_move(chesscxx::SanMove move) { san_move_ = move; }
  void set_final_position(chesscxx::Position position) {
    final_position_ = std::move(position);
  }

  auto raw() const -> const std::string& { return raw_; }
  auto game() const -> const chesscxx::Game& { return game_; }
  auto san_move() const -> const chesscxx::SanMove& { return san_move_; }
  auto uci_move() const -> const chesscxx::UciMove& { return uci_move_; }
  auto final_position() const -> const chesscxx::Position& {
    return final_position_;
  }

 private:
  std::string raw_;
  chesscxx::Game game_;
  chesscxx::UciMove uci_move_;
  chesscxx::SanMove san_move_;
  chesscxx::Position final_position_;
};

template <>
struct YAML::convert<ValidMoveFixture> {
  static auto decode(const Node& node, ValidMoveFixture& rhs) -> bool {
    rhs.set_input(node[0].as<std::string>());

    rhs.set_san_move(
        chesscxx::parse<chesscxx::SanMove>(node[1].as<std::string>()).value());

    rhs.set_uci_move(
        chesscxx::parse<chesscxx::UciMove>(node[2].as<std::string>()).value());

    rhs.set_final_position(
        chesscxx::parse<chesscxx::Position>(node[3].as<std::string>()).value());

    return true;
  }
};

class GameFixture {
 public:
  void set_input(std::string_view raw) {
    raw_ = raw;
    ASSERT_TRUE(chesscxx::parse<chesscxx::Game>(raw)) << std::format(
        "{} {}", raw, chesscxx::parse<chesscxx::Game>(raw).error());
    game_ = chesscxx::parse<chesscxx::Game>(raw).value();
  }

  auto raw() const -> const std::string& { return raw_; }
  auto game() const -> const chesscxx::Game& { return game_; }

 private:
  std::string raw_;
  chesscxx::Game game_;
};

template <>
struct YAML::convert<GameFixture> {
  static auto decode(const Node& node, GameFixture& rhs) -> bool {
    rhs.set_input(node.as<std::string>());
    return true;
  }
};

class FormatFixture {
 public:
  void set_input(std::string_view raw) {
    raw_ = raw;
    ASSERT_TRUE(chesscxx::parse<chesscxx::Game>(raw)) << std::format(
        "{} {}", raw, chesscxx::parse<chesscxx::Game>(raw).error());
    game_ = chesscxx::parse<chesscxx::Game>(raw).value();
  }
  void set_default(std::string_view default_fmt) { default_fmt_ = default_fmt; }
  void set_pgn(std::string_view pgn) { pgn_ = pgn; }
  void set_fen(std::string_view fen) { fen_ = fen; }
  void set_ascii(std::string_view ascii) { ascii_ = ascii; }
  void set_lists(std::string_view lists) { lists_ = lists; }
  void set_rep(std::string_view rep) { rep_ = rep; }

  auto raw() const -> const std::string& { return raw_; }
  auto game() const -> const chesscxx::Game& { return game_; }
  auto default_fmt() -> const std::string& { return default_fmt_; }
  auto pgn() -> const std::string& { return pgn_; }
  auto fen() -> const std::string& { return fen_; }
  auto ascii() -> const std::string& { return ascii_; }
  auto lists() -> const std::string& { return lists_; }
  auto rep() -> const std::string& { return rep_; }

 private:
  std::string raw_;
  chesscxx::Game game_;
  std::string default_fmt_;
  std::string pgn_;
  std::string fen_;
  std::string ascii_;
  std::string lists_;
  std::string rep_;
};

template <>
struct YAML::convert<FormatFixture> {
  static auto decode(const Node& node, FormatFixture& rhs) -> bool {
    int index = 0;
    rhs.set_input(node[index++].as<std::string>());
    rhs.set_default(node[index++].as<std::string>());
    rhs.set_pgn(node[index++].as<std::string>());
    rhs.set_fen(node[index++].as<std::string>());
    rhs.set_ascii(node[index++].as<std::string>());
    rhs.set_lists(node[index++].as<std::string>());
    rhs.set_rep(node[index].as<std::string>());
    return true;
  }
};

class FenFormatFixture {
 public:
  void set_input(std::string_view raw) {
    raw_ = raw;
    ASSERT_TRUE(chesscxx::parse<chesscxx::Game>(raw, chesscxx::parse_as::Fen{}))
        << std::format(
               "{} {}", raw,
               chesscxx::parse<chesscxx::Game>(raw, chesscxx::parse_as::Fen{})
                   .error());
    game_ =
        chesscxx::parse<chesscxx::Game>(raw, chesscxx::parse_as::Fen{}).value();
  }
  void set_default(std::string_view default_fmt) { default_fmt_ = default_fmt; }
  void set_pgn(std::string_view pgn) { pgn_ = pgn; }
  void set_fen(std::string_view fen) { fen_ = fen; }
  void set_ascii(std::string_view ascii) { ascii_ = ascii; }
  void set_lists(std::string_view lists) { lists_ = lists; }
  void set_rep(std::string_view rep) { rep_ = rep; }

  auto raw() const -> const std::string& { return raw_; }
  auto game() const -> const chesscxx::Game& { return game_; }
  auto default_fmt() -> const std::string& { return default_fmt_; }
  auto pgn() -> const std::string& { return pgn_; }
  auto fen() -> const std::string& { return fen_; }
  auto ascii() -> const std::string& { return ascii_; }
  auto lists() -> const std::string& { return lists_; }
  auto rep() -> const std::string& { return rep_; }

 private:
  std::string raw_;
  chesscxx::Game game_;
  std::string default_fmt_;
  std::string pgn_;
  std::string fen_;
  std::string ascii_;
  std::string lists_;
  std::string rep_;
};

template <>
struct YAML::convert<FenFormatFixture> {
  static auto decode(const Node& node, FenFormatFixture& rhs) -> bool {
    int index = 0;
    rhs.set_input(node[index++].as<std::string>());
    rhs.set_default(node[index++].as<std::string>());
    rhs.set_pgn(node[index++].as<std::string>());
    rhs.set_fen(node[index++].as<std::string>());
    rhs.set_ascii(node[index++].as<std::string>());
    rhs.set_lists(node[index++].as<std::string>());
    rhs.set_rep(node[index].as<std::string>());
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
auto GetConfig() { return YAML::LoadFile("data/game.yaml"); }

auto GetPositionFixtures() {
  return GetConfig()["position_fixtures"].as<std::vector<PositionFixture>>();
}

auto GetOutcomeFixtures() {
  return GetConfig()["outcome_fixtures"].as<std::vector<OutcomeFixture>>();
}

auto GetMoveFixtures() {
  return GetConfig()["move_fixtures"].as<std::vector<MoveFixture>>();
}

auto GetRepetitionTrackerFixtures() {
  return GetConfig()["repetition_tracker_fixtures"]
      .as<std::vector<RepetitionTrackerFixture>>();
}

auto GetSanMoveErrorFixtures() {
  return GetConfig()["san_move_error_fixtures"]
      .as<std::vector<SanMoveErrorFixture>>();
}

auto GetUciMoveErrorFixtures() {
  return GetConfig()["uci_move_error_fixtures"]
      .as<std::vector<UciMoveErrorFixture>>();
}

auto GetValidMoveFixtures() {
  return GetConfig()["valid_move_fixtures"].as<std::vector<ValidMoveFixture>>();
}

auto GetGameEqualityFixtures() {
  return GetConfig()["game_equality_fixtures"]
      .as<std::vector<GameEqualityFixture>>();
}

auto GetUniqueFinalPositionFixtures() {
  return GetConfig()["unique_final_position_fixtures"]
      .as<std::vector<GameFixture>>();
}

auto GetUniqueRepetitionTrackerFixtures() {
  return GetConfig()["unique_repetition_tracker_fixtures"]
      .as<std::vector<GameFixture>>();
}

auto BuildGameEqualityFixturePairs() {
  return chesscxx::testing::make_distinct_pairs(GetGameEqualityFixtures());
}

auto GetFormatFixtures() {
  return GetConfig()["format_fixtures"].as<std::vector<FormatFixture>>();
}

auto GetFenFormatFixtures() {
  return GetConfig()["fen_format_fixtures"].as<std::vector<FenFormatFixture>>();
}

auto GetInvalidFenInputs() {
  return GetConfig()["invalid_fen_inputs"].as<std::vector<InvalidFixture>>();
}

auto GetInvalidPgnInputs() {
  return GetConfig()["invalid_pgn_inputs"].as<std::vector<InvalidFixture>>();
}
}  // namespace

class PositionSuite : public ::testing::TestWithParam<PositionFixture> {};
INSTANTIATE_TEST_SUITE_P(PositionTest, PositionSuite,
                         ::testing::ValuesIn(GetPositionFixtures()));

class OutcomeSuite : public ::testing::TestWithParam<OutcomeFixture> {};
INSTANTIATE_TEST_SUITE_P(GameTest, OutcomeSuite,
                         ::testing::ValuesIn(GetOutcomeFixtures()));

class MoveSuite : public ::testing::TestWithParam<MoveFixture> {};
INSTANTIATE_TEST_SUITE_P(GameTest, MoveSuite,
                         ::testing::ValuesIn(GetMoveFixtures()));

class RepetitionTrackerSuite
    : public ::testing::TestWithParam<RepetitionTrackerFixture> {};
INSTANTIATE_TEST_SUITE_P(GameTest, RepetitionTrackerSuite,
                         ::testing::ValuesIn(GetRepetitionTrackerFixtures()));

class SanMoveErrorSuite : public ::testing::TestWithParam<SanMoveErrorFixture> {
};
INSTANTIATE_TEST_SUITE_P(GameTest, SanMoveErrorSuite,
                         ::testing::ValuesIn(GetSanMoveErrorFixtures()));

class UciMoveErrorSuite : public ::testing::TestWithParam<UciMoveErrorFixture> {
};
INSTANTIATE_TEST_SUITE_P(GameTest, UciMoveErrorSuite,
                         ::testing::ValuesIn(GetUciMoveErrorFixtures()));

class ValidMoveSuite : public ::testing::TestWithParam<ValidMoveFixture> {};
INSTANTIATE_TEST_SUITE_P(GameTest, ValidMoveSuite,
                         ::testing::ValuesIn(GetValidMoveFixtures()));

class GameEqualitySuite : public ::testing::TestWithParam<GameEqualityFixture> {
};
INSTANTIATE_TEST_SUITE_P(GameTest, GameEqualitySuite,
                         ::testing::ValuesIn(GetGameEqualityFixtures()));

class GameEqualityPairSuite
    : public ::testing::TestWithParam<
          std::tuple<GameEqualityFixture, GameEqualityFixture>> {};
INSTANTIATE_TEST_SUITE_P(GameTest, GameEqualityPairSuite,
                         ::testing::ValuesIn(BuildGameEqualityFixturePairs()));

class FormatSuite : public ::testing::TestWithParam<FormatFixture> {};
INSTANTIATE_TEST_SUITE_P(GameTest, FormatSuite,
                         ::testing::ValuesIn(GetFormatFixtures()));

class FenFormatSuite : public ::testing::TestWithParam<FenFormatFixture> {};
INSTANTIATE_TEST_SUITE_P(GameTest, FenFormatSuite,
                         ::testing::ValuesIn(GetFenFormatFixtures()));

class InvalidFenInputSuite : public ::testing::TestWithParam<InvalidFixture> {};
INSTANTIATE_TEST_SUITE_P(GameTest, InvalidFenInputSuite,
                         ::testing::ValuesIn(GetInvalidFenInputs()));

class InvalidPgnInputSuite : public ::testing::TestWithParam<InvalidFixture> {};
INSTANTIATE_TEST_SUITE_P(GameTest, InvalidPgnInputSuite,
                         ::testing::ValuesIn(GetInvalidPgnInputs()));

TEST_P(GameEqualitySuite, ComparesEqual) {
  auto games = GetParam().games();

  for (const auto& lhs : games) {
    for (const auto& rhs : games) {
      EXPECT_EQ(lhs, rhs);
      EXPECT_EQ(std::hash<chesscxx::Game>{}(lhs),
                std::hash<chesscxx::Game>{}(rhs));
    }
  }
}

TEST_P(GameEqualityPairSuite, ComparesUnequal) {
  const auto& [lhs_fixture, rhs_fixture] = GetParam();

  for (const auto& lhs : lhs_fixture.games()) {
    for (const auto& rhs : rhs_fixture.games()) EXPECT_NE(lhs, rhs);
  }
}

TEST_P(OutcomeSuite, ReturnsResultAndDrawReasonCorrectly) {
  const auto& fixture = GetParam();
  const auto& game = fixture.game();
  EXPECT_EQ(game.result(), fixture.result());
  EXPECT_EQ(game.drawReason(), fixture.draw_reason());
}

TEST_P(MoveSuite, ReturnsMovesCorrectly) {
  const auto& fixture = GetParam();
  const auto& game = fixture.game();
  EXPECT_EQ(game.sanMoves(), fixture.san_moves());
  EXPECT_EQ(game.uciMoves(), fixture.uci_moves());
}

TEST_P(RepetitionTrackerSuite, BuildRepetitionTrackerCorrectly) {
  auto fixture = GetParam();
  const auto& game = fixture.game();

  std::unordered_set<chesscxx::Position, chesscxx::RepetitionHash,
                     chesscxx::RepetitionEqual>
      seen;

  for (const auto& [position, expected, ignore] : fixture.repetitions()) {
    if (expected > 0) {
      ASSERT_TRUE(game.repetitionTracker().contains(position));
      EXPECT_EQ(game.repetitionTracker().at(position), expected);
      seen.insert(position);
    }
  }

  EXPECT_EQ(seen.size(), game.repetitionTracker().size());
}

TEST_P(RepetitionTrackerSuite, BuildRepetitionTrackerCorrectlyAfterUndo) {
  auto fixture = GetParam();
  auto game = fixture.game();
  game.undoMove();

  std::unordered_set<chesscxx::Position, chesscxx::RepetitionHash,
                     chesscxx::RepetitionEqual>
      seen;

  for (const auto& [position, ignore, expected] : fixture.repetitions()) {
    if (expected > 0) {
      ASSERT_TRUE(game.repetitionTracker().contains(position));
      EXPECT_EQ(game.repetitionTracker().at(position), expected);
      seen.insert(position);
    }
  }

  EXPECT_EQ(seen.size(), game.repetitionTracker().size());
}

TEST_P(RepetitionTrackerSuite, UndoAllMovesCorrectly) {
  auto game = chesscxx::Game();

  while (game.currentPosition() != game.initialPosition()) game.undoMove();

  EXPECT_EQ(game.currentPosition(), game.initialPosition());
  EXPECT_TRUE(game.sanMoves().empty());
  EXPECT_TRUE(game.uciMoves().empty());
  EXPECT_EQ(game.repetitionTracker().size(), 1);
  EXPECT_EQ(game.repetitionTracker().at(game.initialPosition()), 1);
}

TEST_P(RepetitionTrackerSuite, ResetCorrectly) {
  auto game = chesscxx::Game();

  game.reset();

  EXPECT_EQ(game.currentPosition(), game.initialPosition());
  EXPECT_TRUE(game.sanMoves().empty());
  EXPECT_TRUE(game.uciMoves().empty());
  EXPECT_EQ(game.repetitionTracker().size(), 1);
  EXPECT_EQ(game.repetitionTracker().at(game.initialPosition()), 1);
}

TEST_P(PositionSuite, ConstructFromInitialPositionCorrectly) {
  auto position = GetParam().position();
  auto game = chesscxx::Game(position);
  EXPECT_EQ(game.initialPosition(), game.currentPosition());
  EXPECT_EQ(game.initialPosition(), position);
  EXPECT_FALSE(game.startsFromDefaultPosition());

  EXPECT_EQ(game.piecePlacement(), position.piecePlacement());
  EXPECT_EQ(game.activeColor(), position.activeColor());
  EXPECT_EQ(game.enPassantTargetSquare(), position.enPassantTargetSquare());
  EXPECT_EQ(game.legalEnPassantTargetSquare(),
            position.legalEnPassantTargetSquare());
  EXPECT_EQ(game.halfmoveClock(), position.halfmoveClock());
  EXPECT_EQ(game.fullmoveNumber(), position.fullmoveNumber());
  EXPECT_EQ(game.castlingRights(), position.castlingRights());
  EXPECT_TRUE(game.uciMoves().empty());
  EXPECT_TRUE(game.sanMoves().empty());
  EXPECT_EQ(game.repetitionTracker().size(), 1);
  EXPECT_EQ(game.repetitionTracker().at(position), 1);
}

TEST_P(SanMoveErrorSuite, ReturnsCorrectMoveError) {
  const auto& fixture = GetParam();
  auto game = fixture.game();

  auto move = game.move(fixture.move());
  EXPECT_EQ(move.error(), fixture.error());
}

TEST_P(UciMoveErrorSuite, ReturnsCorrectMoveError) {
  const auto& fixture = GetParam();
  auto game = fixture.game();

  auto move = game.move(fixture.move());
  EXPECT_EQ(move.error(), fixture.error());
}

TEST_P(ValidMoveSuite, UpdatePositionCorrectly) {
  const auto& fixture = GetParam();
  auto game = fixture.game();

  auto move = game.move(fixture.san_move());
  ASSERT_TRUE(move);
  EXPECT_EQ(game.currentPosition(), fixture.final_position());

  game = fixture.game();
  move = game.move(fixture.uci_move());
  ASSERT_TRUE(move);
  EXPECT_EQ(game.currentPosition(), fixture.final_position());
}

TEST_P(ValidMoveSuite, UndoMoveCorrectly) {
  const auto& fixture = GetParam();
  auto game = fixture.game();
  auto initial_position = game.currentPosition();

  auto move = game.move(fixture.san_move());
  ASSERT_TRUE(move);
  EXPECT_NE(game.currentPosition(), initial_position);
  game.undoMove();
  EXPECT_EQ(game.currentPosition(), initial_position);

  game = fixture.game();
  move = game.move(fixture.uci_move());
  ASSERT_TRUE(move);
  EXPECT_NE(game.currentPosition(), initial_position);
  game.undoMove();
  EXPECT_EQ(game.currentPosition(), initial_position);
}

TEST_P(FormatSuite, FormatProducesExpectOutput) {
  auto fixture = GetParam();
  EXPECT_EQ(std::format("{}", fixture.game()), fixture.default_fmt());
  EXPECT_EQ(std::format("{:pgn}", fixture.game()), fixture.pgn());
  EXPECT_EQ(std::format("{:fen}", fixture.game()), fixture.fen());
  EXPECT_EQ(std::format("{:ascii}", fixture.game()), fixture.ascii());
  EXPECT_EQ(std::format("{:lists}", fixture.game()), fixture.lists());
  EXPECT_EQ(std::format("{:rep}", fixture.game()), fixture.rep());
}

TEST_P(FenFormatSuite, FormatProducesExpectOutput) {
  auto fixture = GetParam();
  EXPECT_EQ(std::format("{}", fixture.game()), fixture.default_fmt());
  EXPECT_EQ(std::format("{:pgn}", fixture.game()), fixture.pgn());
  EXPECT_EQ(std::format("{:fen}", fixture.game()), fixture.fen());
  EXPECT_EQ(std::format("{:ascii}", fixture.game()), fixture.ascii());
  EXPECT_EQ(std::format("{:lists}", fixture.game()), fixture.lists());
  EXPECT_EQ(std::format("{:rep}", fixture.game()), fixture.rep());
}

TEST_P(InvalidFenInputSuite, ParseHandlesInvalidInputCorrectly) {
  const auto& fixture = GetParam();
  auto result =
      chesscxx::parse<chesscxx::Game>(fixture.raw(), chesscxx::parse_as::Fen{});
  EXPECT_EQ(result.error(), fixture.error());
}

TEST_P(InvalidPgnInputSuite, ParseHandlesInvalidInputCorrectly) {
  const auto& fixture = GetParam();
  auto result =
      chesscxx::parse<chesscxx::Game>(fixture.raw(), chesscxx::parse_as::Pgn{});
  ASSERT_FALSE(result.has_value());
  EXPECT_EQ(result.error(), fixture.error());

  auto default_result = chesscxx::parse<chesscxx::Game>(fixture.raw());
  ASSERT_FALSE(default_result.has_value());
  EXPECT_EQ(default_result.error(), fixture.error());
}

TEST(GameTest, FormatProducesUniqueStrings) {
  using CollisionMap =
      std::unordered_map<std::string, std::vector<chesscxx::Game>>;
  CollisionMap fmt_collisions;
  CollisionMap pgn_fmt_collisions;

  auto assert_unique_insertion = [](auto& map, const auto& key,
                                    const auto& val) {
    map[key].push_back(val);
    EXPECT_EQ(map[key].size(), 1) << std::format("game={}, map={}", val, map);
  };

  std::ranges::for_each(GetGameEqualityFixtures(), [&](const auto& fixture) {
    const auto& game = fixture.games().front();
    assert_unique_insertion(fmt_collisions, std::format("{}", game), game);
    assert_unique_insertion(pgn_fmt_collisions, std::format("{:pgn}", game),
                            game);
  });
}

TEST(GameTest, FormatProducesUniqueStringsForUniqueFinalPosition) {
  using CollisionMap =
      std::unordered_map<std::string, std::vector<chesscxx::Game>>;
  CollisionMap fen_fmt_collisions;
  CollisionMap ascii_fmt_collisions;
  CollisionMap lists_fmt_collisions;

  auto assert_unique_insertion = [](auto& map, const auto& key,
                                    const auto& val) {
    map[key].push_back(val);
    EXPECT_EQ(map[key].size(), 1) << std::format("game={}, map={}", val, map);
  };

  std::ranges::for_each(
      GetUniqueFinalPositionFixtures(), [&](const auto& fixture) {
        const auto& game = fixture.game();
        assert_unique_insertion(fen_fmt_collisions, std::format("{:fen}", game),
                                game);
        assert_unique_insertion(ascii_fmt_collisions,
                                std::format("{:ascii}", game), game);
        assert_unique_insertion(lists_fmt_collisions,
                                std::format("{:lists}", game), game);
      });
}

TEST(GameTest, FormatProducesUniqueStringsForUniqueRepetitionTracker) {
  using CollisionMap =
      std::unordered_map<std::string, std::vector<chesscxx::Game>>;
  CollisionMap rep_fmt_collisions;

  auto assert_unique_insertion = [](auto& map, const auto& key,
                                    const auto& val) {
    map[key].push_back(val);
    EXPECT_EQ(map[key].size(), 1) << std::format("game={}, map={}", val, map);
  };

  std::ranges::for_each(
      GetUniqueRepetitionTrackerFixtures(), [&](const auto& fixture) {
        const auto& game = fixture.game();
        assert_unique_insertion(rep_fmt_collisions, std::format("{:rep}", game),
                                game);
      });
}

TEST(GameTest, HashProducesFewCollisions) {
  // Adjust based on expectations for the test set.
  constexpr int kMaxCollisions = 1;
  std::unordered_map<size_t, std::vector<chesscxx::Game>> hash_counter;

  std::ranges::for_each(GetGameEqualityFixtures(), [&](const auto& fixture) {
    const auto& game = fixture.games().front();
    auto hash = std::hash<chesscxx::Game>{}(game);
    hash_counter[hash].push_back(game);
    auto collisions = hash_counter[hash].size();
    EXPECT_LE(collisions, kMaxCollisions)
        << std::format("game={}, hash_counter={}", game, hash_counter);
  });
}

TEST(GameTest, UndoMoveCorrectlyForDefaultGame) {
  auto game = chesscxx::Game();

  EXPECT_EQ(game.currentPosition(), game.initialPosition());
  EXPECT_TRUE(game.sanMoves().empty());
  EXPECT_TRUE(game.uciMoves().empty());
  EXPECT_EQ(game.repetitionTracker().size(), 1);
  EXPECT_EQ(game.repetitionTracker().at(game.initialPosition()), 1);

  game.undoMove();
  EXPECT_EQ(game.currentPosition(), game.initialPosition());
  EXPECT_TRUE(game.sanMoves().empty());
  EXPECT_TRUE(game.uciMoves().empty());
  EXPECT_EQ(game.repetitionTracker().size(), 1);
  EXPECT_EQ(game.repetitionTracker().at(game.initialPosition()), 1);
}

TEST(GameTest, ResetCorrectlyForDefaultGame) {
  auto game = chesscxx::Game();

  EXPECT_EQ(game.currentPosition(), game.initialPosition());
  EXPECT_TRUE(game.sanMoves().empty());
  EXPECT_TRUE(game.uciMoves().empty());
  EXPECT_EQ(game.repetitionTracker().size(), 1);
  EXPECT_EQ(game.repetitionTracker().at(game.initialPosition()), 1);

  game.reset();
  EXPECT_EQ(game.currentPosition(), game.initialPosition());
  EXPECT_TRUE(game.sanMoves().empty());
  EXPECT_TRUE(game.uciMoves().empty());
  EXPECT_EQ(game.repetitionTracker().size(), 1);
  EXPECT_EQ(game.repetitionTracker().at(game.initialPosition()), 1);
}

TEST(GameTest, DefaultConstructionResultsInDefaultsGame) {
  chesscxx::Game const game;
  chesscxx::Position const default_position;

  EXPECT_EQ(game.initialPosition(), game.currentPosition());
  EXPECT_EQ(game.initialPosition(), default_position);
  EXPECT_TRUE(game.startsFromDefaultPosition());

  EXPECT_EQ(game.piecePlacement(), default_position.piecePlacement());
  EXPECT_EQ(game.activeColor(), default_position.activeColor());
  EXPECT_EQ(game.enPassantTargetSquare(),
            default_position.enPassantTargetSquare());
  EXPECT_EQ(game.legalEnPassantTargetSquare(),
            default_position.legalEnPassantTargetSquare());
  EXPECT_EQ(game.halfmoveClock(), default_position.halfmoveClock());
  EXPECT_EQ(game.fullmoveNumber(), default_position.fullmoveNumber());
  EXPECT_EQ(game.castlingRights(), default_position.castlingRights());
  EXPECT_TRUE(game.uciMoves().empty());
  EXPECT_TRUE(game.sanMoves().empty());
  EXPECT_EQ(game.repetitionTracker().size(), 1);
  EXPECT_EQ(game.repetitionTracker().at(default_position), 1);
  EXPECT_FALSE(game.result());
  EXPECT_FALSE(game.drawReason());
}

TEST(GameTest, ConstructFromInitialPositionCorrectly) {
  chesscxx::Position const default_position;
  chesscxx::Game const game(default_position);

  EXPECT_EQ(game.initialPosition(), game.currentPosition());
  EXPECT_EQ(game.initialPosition(), default_position);
  EXPECT_TRUE(game.startsFromDefaultPosition());

  EXPECT_EQ(game.piecePlacement(), default_position.piecePlacement());
  EXPECT_EQ(game.activeColor(), default_position.activeColor());
  EXPECT_EQ(game.enPassantTargetSquare(),
            default_position.enPassantTargetSquare());
  EXPECT_EQ(game.legalEnPassantTargetSquare(),
            default_position.legalEnPassantTargetSquare());
  EXPECT_EQ(game.halfmoveClock(), default_position.halfmoveClock());
  EXPECT_EQ(game.fullmoveNumber(), default_position.fullmoveNumber());
  EXPECT_EQ(game.castlingRights(), default_position.castlingRights());
  EXPECT_TRUE(game.uciMoves().empty());
  EXPECT_TRUE(game.sanMoves().empty());
  EXPECT_EQ(game.repetitionTracker().size(), 1);
  EXPECT_EQ(game.repetitionTracker().at(default_position), 1);
  EXPECT_FALSE(game.result());
  EXPECT_FALSE(game.drawReason());
}
