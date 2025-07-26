#include <chesscxx/castling_side.h>
#include <chesscxx/check_indicator.h>
#include <chesscxx/file.h>
#include <chesscxx/parse.h>
#include <chesscxx/parse_error.h>
#include <chesscxx/partial_square.h>
#include <chesscxx/piece_type.h>
#include <chesscxx/rank.h>
#include <chesscxx/san_move.h>
#include <chesscxx/square.h>
#include <gtest/gtest.h>

#include <algorithm>
#include <array>
#include <cassert>
#include <format>
#include <magic_enum/magic_enum.hpp>
#include <optional>
#include <ranges>
#include <string_view>
#include <tuple>
#include <variant>

#include "test_helper.h"

constexpr auto kAllOptPromotions = chesscxx::testing::make_optional_enum_array<
    chesscxx::PromotablePieceType>();

constexpr static auto kTestFiles =
    std::array{chesscxx::File::kA, chesscxx::File::kE, chesscxx::File::kH};

constexpr static auto kTestRanks =
    std::array{chesscxx::Rank::k8, chesscxx::Rank::k4, chesscxx::Rank::k1};

constexpr static auto kTestOptFiles =
    chesscxx::testing::make_optional_array_from_range(kTestFiles);
constexpr static auto kTestOptRanks =
    chesscxx::testing::make_optional_array_from_range(kTestRanks);

constexpr static auto kTestPartialSquares =
    std::views::cartesian_product(kTestOptRanks, kTestOptFiles) |
    std::views::transform([](const auto& product) {
      const auto& [rank, file] = product;
      return chesscxx::PartialSquare(file, rank);
    });

constexpr static auto kTestSquares =
    std::views::cartesian_product(kTestRanks, kTestFiles) |
    std::views::transform([](const auto& product) {
      const auto& [rank, file] = product;
      return chesscxx::Square(file, rank);
    });

constexpr static auto kAllOptCheckIndicators =
    chesscxx::testing::make_optional_enum_array<chesscxx::CheckIndicator>();

constexpr static auto kAllSanCastlingMoves =
    std::views::cartesian_product(
        magic_enum::enum_values<chesscxx::CastlingSide>(),
        kAllOptCheckIndicators) |
    std::views::transform([](const auto& product) {
      const auto& [side, check_indicator] = product;
      return chesscxx::SanCastlingMove(side, check_indicator);
    });

constexpr static auto kAllBooleans = std::array{true, false};

constexpr static auto kTestSanNormalMoves =
    std::views::cartesian_product(
        magic_enum::enum_values<chesscxx::PieceType>(), kTestPartialSquares,
        kAllBooleans, kTestSquares, kAllOptPromotions, kAllOptCheckIndicators) |
    std::views::transform([](const auto& product) {
      const auto& [piece_type, origin, is_capture, destination, promotion,
                   check_indicator] = product;
      return chesscxx::SanNormalMove(piece_type, origin, is_capture,
                                     destination, promotion, check_indicator);
    });

namespace {
constexpr auto makeTestSanMoves() noexcept {
  constexpr auto kSize =
      kAllSanCastlingMoves.size() + kTestSanNormalMoves.size();
  std::array<chesscxx::SanMove, kSize> arr;
  auto result = std::ranges::copy(kAllSanCastlingMoves.begin(),
                                  kAllSanCastlingMoves.end(), arr.begin());
  std::ranges::copy(kTestSanNormalMoves.begin(), kTestSanNormalMoves.end(),
                    result.out);
  return arr;
}
}  // namespace

const static auto kTestSanMoves = makeTestSanMoves();

// SanCastlingMove

TEST(SanCastlingMoveTest, DefaultConstructionResultsInValidSanCastlingMove) {
  chesscxx::SanCastlingMove const san_castling;
  EXPECT_TRUE(
      magic_enum::enum_contains<chesscxx::CastlingSide>(san_castling.side));
  EXPECT_EQ(san_castling.check_indicator, std::nullopt);
}

TEST(SanCastlingMoveTest, RoundTripConversionIsSuccessful) {
  std::ranges::for_each(kAllSanCastlingMoves, [](const auto& san_castling) {
    EXPECT_EQ(san_castling, chesscxx::parse<chesscxx::SanCastlingMove>(
                                std::format("{}", san_castling)));
  });
}

TEST(SanCastlingMoveTest, FormatProducesExpectedOutput) {
  using chesscxx::SanCastlingMove;
  using enum chesscxx::CastlingSide;
  using enum chesscxx::CheckIndicator;

  constexpr std::array<std::tuple<chesscxx::SanCastlingMove, std::string_view>,
                       kAllSanCastlingMoves.size()>
      kFixtures = {{
          {SanCastlingMove(kQueenside, std::nullopt), "O-O-O"},
          {SanCastlingMove(kQueenside, kCheck), "O-O-O+"},
          {SanCastlingMove(kQueenside, kCheckmate), "O-O-O#"},
          {SanCastlingMove(kKingside, std::nullopt), "O-O"},
          {SanCastlingMove(kKingside, kCheck), "O-O+"},
          {SanCastlingMove(kKingside, kCheckmate), "O-O#"},
      }};

  for (const auto& [rank, str] : kFixtures) {
    EXPECT_EQ(std::format("{}", rank), str);
  }
}

TEST(SanCastlingMoveTest, ParseHandlesInvalidInputCorrectly) {
  EXPECT_EQ(chesscxx::parse<chesscxx::SanCastlingMove>("e4").error(),
            chesscxx::ParseError::kInvalidSanCastling);
  EXPECT_EQ(chesscxx::parse<chesscxx::SanCastlingMove>("O-0").error(),
            chesscxx::ParseError::kInvalidSanCastling);
  EXPECT_EQ(chesscxx::parse<chesscxx::SanCastlingMove>("0-0").error(),
            chesscxx::ParseError::kInvalidSanCastling);
  EXPECT_EQ(chesscxx::parse<chesscxx::SanCastlingMove>("o-o").error(),
            chesscxx::ParseError::kInvalidSanCastling);
  EXPECT_EQ(chesscxx::parse<chesscxx::SanCastlingMove>("x").error(),
            chesscxx::ParseError::kInvalidSanCastling);
  EXPECT_EQ(chesscxx::parse<chesscxx::SanCastlingMove>("O-O-0").error(),
            chesscxx::ParseError::kExpectingEndOfString);
}

// SanNormalMove

TEST(SanNormalMoveTest, DefaultConstructionResultsInValidSanNormalMove) {
  chesscxx::SanNormalMove const san_normal;
  EXPECT_TRUE(
      magic_enum::enum_contains<chesscxx::PieceType>(san_normal.piece_type));
  EXPECT_EQ(san_normal.origin.file, std::nullopt);
  EXPECT_EQ(san_normal.origin.rank, std::nullopt);
  EXPECT_EQ(san_normal.is_capture, false);
  EXPECT_EQ(chesscxx::index(san_normal.destination), 0);
  EXPECT_EQ(san_normal.promotion, std::nullopt);
  EXPECT_EQ(san_normal.check_indicator, std::nullopt);
}

TEST(SanNormalMoveTest, RoundTripConversionIsSuccessful) {
  std::ranges::for_each(kTestSanNormalMoves, [](const auto& san_normal) {
    EXPECT_EQ(san_normal, chesscxx::parse<chesscxx::SanNormalMove>(
                              std::format("{}", san_normal)));
  });
}

TEST(SanNormalMoveTest, FormatProducesNonEmptyStrings) {
  std::ranges::for_each(kTestSanNormalMoves, [](const auto& san_normal) {
    EXPECT_FALSE(std::format("{}", san_normal).empty());
  });
}

TEST(SanNormalMoveTest, FormatProducesExpectedOutput) {
  using chesscxx::PartialSquare;
  using chesscxx::PromotablePieceType;
  using chesscxx::SanNormalMove;
  using chesscxx::Square;
  using enum chesscxx::PieceType;
  using enum chesscxx::File;
  using enum chesscxx::Rank;
  using enum chesscxx::CheckIndicator;

  EXPECT_EQ(std::format("{}",
                        SanNormalMove{
                            .piece_type = kPawn,
                            .destination = Square(kE, k4),
                        }),
            "e4");

  EXPECT_EQ(std::format("{}",
                        SanNormalMove{
                            .piece_type = kPawn,
                            .destination = Square(kA, k8),
                            .promotion = PromotablePieceType::kQueen,
                        }),
            "a8=Q");

  EXPECT_EQ(std::format("{}",
                        SanNormalMove{
                            .piece_type = kQueen,
                            .origin = PartialSquare(kD, std::nullopt),
                            .destination = Square(kE, k4),
                        }),
            "Qde4");

  EXPECT_EQ(std::format("{}",
                        SanNormalMove{
                            .piece_type = kKnight,
                            .origin = PartialSquare(std::nullopt, k3),
                            .destination = Square(kD, k5),
                        }),
            "N3d5");

  EXPECT_EQ(std::format("{}",
                        SanNormalMove{
                            .piece_type = kBishop,
                            .origin = PartialSquare(kA, k3),
                            .destination = Square(kF, k8),
                        }),
            "Ba3f8");

  EXPECT_EQ(std::format("{}",
                        SanNormalMove{
                            .piece_type = kRook,
                            .is_capture = true,
                            .destination = Square(kH, k5),
                            .check_indicator = kCheck,
                        }),
            "Rxh5+");

  EXPECT_EQ(std::format("{}",
                        SanNormalMove{
                            .piece_type = kKing,
                            .is_capture = true,
                            .destination = Square(kF, k7),
                            .check_indicator = kCheckmate,
                        }),
            "Kxf7#");
}

TEST(SanNormalMoveTest, PawnSymbolIsOptionalDuringParsing) {
  auto parsed_p = chesscxx::parse<chesscxx::SanNormalMove>("Pe4");
  auto parsed = chesscxx::parse<chesscxx::SanNormalMove>("e4");

  EXPECT_EQ(parsed_p, parsed);

  parsed_p = chesscxx::parse<chesscxx::SanNormalMove>("Pd3xe4#");
  parsed = chesscxx::parse<chesscxx::SanNormalMove>("d3xe4#");

  EXPECT_EQ(parsed_p, parsed);

  parsed_p = chesscxx::parse<chesscxx::SanNormalMove>("P3xe4+");
  parsed = chesscxx::parse<chesscxx::SanNormalMove>("3xe4+");

  EXPECT_EQ(parsed_p, parsed);
}

TEST(SanNormalMoveTest, ParseHandlesInvalidInputCorrectly) {
  EXPECT_EQ(chesscxx::parse<chesscxx::SanNormalMove>("O-O").error(),
            chesscxx::ParseError::kInvalidFile);
  EXPECT_EQ(chesscxx::parse<chesscxx::SanNormalMove>("e4=K").error(),
            chesscxx::ParseError::kInvalidPromotablePieceType);
  EXPECT_EQ(chesscxx::parse<chesscxx::SanNormalMove>("e4=").error(),
            chesscxx::ParseError::kInvalidPromotablePieceType);
  EXPECT_EQ(chesscxx::parse<chesscxx::SanNormalMove>("=Q").error(),
            chesscxx::ParseError::kInvalidFile);
  EXPECT_EQ(chesscxx::parse<chesscxx::SanNormalMove>("e4xe").error(),
            chesscxx::ParseError::kInvalidRank);
  EXPECT_EQ(chesscxx::parse<chesscxx::SanNormalMove>("e4x").error(),
            chesscxx::ParseError::kInvalidFile);
  EXPECT_EQ(chesscxx::parse<chesscxx::SanNormalMove>("").error(),
            chesscxx::ParseError::kInvalidFile);
  EXPECT_EQ(chesscxx::parse<chesscxx::SanNormalMove>("e").error(),
            chesscxx::ParseError::kInvalidFile);
  EXPECT_EQ(chesscxx::parse<chesscxx::SanNormalMove>("4").error(),
            chesscxx::ParseError::kInvalidFile);
  EXPECT_EQ(chesscxx::parse<chesscxx::SanNormalMove>("?").error(),
            chesscxx::ParseError::kInvalidFile);
  EXPECT_EQ(chesscxx::parse<chesscxx::SanNormalMove>("e?").error(),
            chesscxx::ParseError::kInvalidFile);
  EXPECT_EQ(chesscxx::parse<chesscxx::SanNormalMove>("e4?").error(),
            chesscxx::ParseError::kExpectingEndOfString);
}

// SanMove

struct SanMoveDefaultTester {
  void operator()(chesscxx::SanNormalMove move) const {
    auto is_equal = [](const chesscxx::PartialSquare& lhs,
                       const chesscxx::PartialSquare& rhs) -> bool {
      return lhs.file == rhs.file && lhs.rank == rhs.rank;
    };

    const chesscxx::PartialSquare partial = {.file = std::nullopt,
                                             .rank = std::nullopt};

    EXPECT_TRUE(
        magic_enum::enum_contains<chesscxx::PieceType>(move.piece_type));
    EXPECT_TRUE(is_equal(move.origin, partial));
    EXPECT_EQ(move.is_capture, false);
    EXPECT_EQ(chesscxx::index(move.destination), 0);
    EXPECT_EQ(move.promotion, std::nullopt);
    EXPECT_EQ(move.check_indicator, std::nullopt);
  }
  template <typename MoveType>
  void operator()(MoveType /*unused*/) const {
    FAIL() << "SanMove should default to SanNormalMove.";
  }
};

TEST(SanMoveTest, DefaultConstructionResultsInValidSanNormalMove) {
  chesscxx::SanMove const san;
  std::visit(SanMoveDefaultTester{}, san);
}

TEST(SanMoveTest, RoundTripConversionIsSuccessful) {
  std::ranges::for_each(kTestSanMoves, [](const auto& san) {
    EXPECT_EQ(san, chesscxx::parse<chesscxx::SanMove>(std::format("{}", san)));
  });
}

TEST(SanMoveTest, FormatProducesNonEmptyStrings) {
  std::ranges::for_each(kTestSanMoves, [](const auto& san) {
    EXPECT_FALSE(std::format("{}", san).empty());
  });
}

TEST(SanMoveTest, ParseHandlesInvalidInputCorrectly) {
  EXPECT_EQ(chesscxx::parse<chesscxx::SanMove>("O-0").error(),
            chesscxx::ParseError::kInvalidFile);
  EXPECT_EQ(chesscxx::parse<chesscxx::SanMove>("0-0").error(),
            chesscxx::ParseError::kInvalidFile);
  EXPECT_EQ(chesscxx::parse<chesscxx::SanMove>("o-o").error(),
            chesscxx::ParseError::kInvalidFile);
  EXPECT_EQ(chesscxx::parse<chesscxx::SanMove>("x").error(),
            chesscxx::ParseError::kInvalidFile);
  EXPECT_EQ(chesscxx::parse<chesscxx::SanMove>("O-O-0").error(),
            chesscxx::ParseError::kExpectingEndOfString);

  EXPECT_EQ(chesscxx::parse<chesscxx::SanMove>("e4=K").error(),
            chesscxx::ParseError::kInvalidPromotablePieceType);
  EXPECT_EQ(chesscxx::parse<chesscxx::SanMove>("e4=").error(),
            chesscxx::ParseError::kInvalidPromotablePieceType);
  EXPECT_EQ(chesscxx::parse<chesscxx::SanMove>("=Q").error(),
            chesscxx::ParseError::kInvalidFile);
  EXPECT_EQ(chesscxx::parse<chesscxx::SanMove>("e4xe").error(),
            chesscxx::ParseError::kInvalidRank);
  EXPECT_EQ(chesscxx::parse<chesscxx::SanMove>("e4x").error(),
            chesscxx::ParseError::kInvalidFile);
  EXPECT_EQ(chesscxx::parse<chesscxx::SanMove>("").error(),
            chesscxx::ParseError::kInvalidFile);
  EXPECT_EQ(chesscxx::parse<chesscxx::SanMove>("e").error(),
            chesscxx::ParseError::kInvalidFile);
  EXPECT_EQ(chesscxx::parse<chesscxx::SanMove>("4").error(),
            chesscxx::ParseError::kInvalidFile);
  EXPECT_EQ(chesscxx::parse<chesscxx::SanMove>("?").error(),
            chesscxx::ParseError::kInvalidFile);
  EXPECT_EQ(chesscxx::parse<chesscxx::SanMove>("e?").error(),
            chesscxx::ParseError::kInvalidFile);
  EXPECT_EQ(chesscxx::parse<chesscxx::SanMove>("e4?").error(),
            chesscxx::ParseError::kExpectingEndOfString);
}
