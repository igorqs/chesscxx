#include <chesskit/castling_side.h>
#include <chesskit/check_indicator.h>
#include <chesskit/file.h>
#include <chesskit/parse.h>
#include <chesskit/parse_error.h>
#include <chesskit/partial_square.h>
#include <chesskit/piece_type.h>
#include <chesskit/rank.h>
#include <chesskit/san_move.h>
#include <chesskit/square.h>
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
#include <type_traits>
#include <variant>

#include "test_helper.h"

constexpr auto kAllOptPromotions = chesskit::testing::make_optional_enum_array<
    chesskit::PromotablePieceType>();

constexpr static auto kTestFiles =
    std::array{chesskit::File::kA, chesskit::File::kE, chesskit::File::kH};

constexpr static auto kTestRanks =
    std::array{chesskit::Rank::k8, chesskit::Rank::k4, chesskit::Rank::k1};

constexpr static auto kTestOptFiles =
    chesskit::testing::make_optional_array_from_range(kTestFiles);
constexpr static auto kTestOptRanks =
    chesskit::testing::make_optional_array_from_range(kTestRanks);

constexpr static auto kTestPartialSquares =
    std::views::cartesian_product(kTestOptRanks, kTestOptFiles) |
    std::views::transform([](const auto& product) {
      const auto& [rank, file] = product;
      return chesskit::PartialSquare(file, rank);
    });

constexpr static auto kTestSquares =
    std::views::cartesian_product(kTestRanks, kTestFiles) |
    std::views::transform([](const auto& product) {
      const auto& [rank, file] = product;
      return chesskit::Square(file, rank);
    });

constexpr static auto kAllOptCheckIndicators =
    chesskit::testing::make_optional_enum_array<chesskit::CheckIndicator>();

constexpr static auto kAllSanCastlingMoves =
    std::views::cartesian_product(
        magic_enum::enum_values<chesskit::CastlingSide>(),
        kAllOptCheckIndicators) |
    std::views::transform([](const auto& product) {
      const auto& [side, check_indicator] = product;
      return chesskit::SanCastlingMove(side, check_indicator);
    });

constexpr static auto kAllBooleans = std::array{true, false};

constexpr static auto kTestSanNormalMoves =
    std::views::cartesian_product(
        magic_enum::enum_values<chesskit::PieceType>(), kTestPartialSquares,
        kAllBooleans, kTestSquares, kAllOptPromotions, kAllOptCheckIndicators) |
    std::views::transform([](const auto& product) {
      const auto& [piece_type, origin, is_capture, destination, promotion,
                   check_indicator] = product;
      return chesskit::SanNormalMove(piece_type, origin, is_capture,
                                     destination, promotion, check_indicator);
    });

const static auto kTestSanMoves = [] {
  using chesskit::SanMove;
  constexpr auto kSize =
      kAllSanCastlingMoves.size() + kTestSanNormalMoves.size();
  std::array<SanMove, kSize> arr;
  auto result = std::ranges::copy(kAllSanCastlingMoves.begin(),
                                  kAllSanCastlingMoves.end(), arr.begin());
  std::ranges::copy(kTestSanNormalMoves.begin(), kTestSanNormalMoves.end(),
                    result.out);
  return arr;
}();

// SanCastlingMove

TEST(SanCastlingMoveTest, DefaultConstructionResultsInValidSanCastlingMove) {
  chesskit::SanCastlingMove san_castling;
  EXPECT_TRUE(
      magic_enum::enum_contains<chesskit::CastlingSide>(san_castling.side));
  EXPECT_EQ(san_castling.check_indicator, std::nullopt);
}

TEST(SanCastlingMoveTest, RoundTripConversionIsSuccessful) {
  std::ranges::for_each(kAllSanCastlingMoves, [](const auto& san_castling) {
    EXPECT_EQ(san_castling, chesskit::parse<chesskit::SanCastlingMove>(
                                std::format("{}", san_castling)));
  });
}

TEST(SanCastlingMoveTest, FormatProducesExpectedOutput) {
  using chesskit::SanCastlingMove;
  using enum chesskit::CastlingSide;
  using enum chesskit::CheckIndicator;

  constexpr std::array<std::tuple<chesskit::SanCastlingMove, std::string_view>,
                       kAllSanCastlingMoves.size()>
      kFixtures = {{
          {SanCastlingMove(kQueenside, std::nullopt), "O-O-O"},
          {SanCastlingMove(kQueenside, kCheck), "O-O-O+"},
          {SanCastlingMove(kQueenside, kCheckmate), "O-O-O#"},
          {SanCastlingMove(kKingside, std::nullopt), "O-O"},
          {SanCastlingMove(kKingside, kCheck), "O-O+"},
          {SanCastlingMove(kKingside, kCheckmate), "O-O#"},
      }};

  for (const auto& [rank, sv] : kFixtures)
    EXPECT_EQ(std::format("{}", rank), sv);
}

TEST(SanCastlingMoveTest, ParseHandlesInvalidInputCorrectly) {
  EXPECT_EQ(chesskit::parse<chesskit::SanCastlingMove>("e4").error(),
            chesskit::ParseError::kInvalidSanCastling);
  EXPECT_EQ(chesskit::parse<chesskit::SanCastlingMove>("O-0").error(),
            chesskit::ParseError::kInvalidSanCastling);
  EXPECT_EQ(chesskit::parse<chesskit::SanCastlingMove>("0-0").error(),
            chesskit::ParseError::kInvalidSanCastling);
  EXPECT_EQ(chesskit::parse<chesskit::SanCastlingMove>("o-o").error(),
            chesskit::ParseError::kInvalidSanCastling);
  EXPECT_EQ(chesskit::parse<chesskit::SanCastlingMove>("x").error(),
            chesskit::ParseError::kInvalidSanCastling);
  EXPECT_EQ(chesskit::parse<chesskit::SanCastlingMove>("O-O-0").error(),
            chesskit::ParseError::kExpectingEndOfString);
}

// SanNormalMove

TEST(SanNormalMoveTest, DefaultConstructionResultsInValidSanNormalMove) {
  chesskit::SanNormalMove san_normal;
  EXPECT_TRUE(
      magic_enum::enum_contains<chesskit::PieceType>(san_normal.pieceType));
  EXPECT_EQ(san_normal.origin.file, std::nullopt);
  EXPECT_EQ(san_normal.origin.rank, std::nullopt);
  EXPECT_EQ(san_normal.isCapture, false);
  EXPECT_EQ(chesskit::index(san_normal.destination), 0);
  EXPECT_EQ(san_normal.promotion, std::nullopt);
  EXPECT_EQ(san_normal.check_indicator, std::nullopt);
}

TEST(SanNormalMoveTest, RoundTripConversionIsSuccessful) {
  std::ranges::for_each(kTestSanNormalMoves, [](const auto& san_normal) {
    EXPECT_EQ(san_normal, chesskit::parse<chesskit::SanNormalMove>(
                              std::format("{}", san_normal)));
  });
}

TEST(SanNormalMoveTest, FormatProducesNonEmptyStrings) {
  std::ranges::for_each(kTestSanNormalMoves, [](const auto& san_normal) {
    EXPECT_FALSE(std::format("{}", san_normal).empty());
  });
}

TEST(SanNormalMoveTest, FormatProducesExpectedOutput) {
  using chesskit::PartialSquare;
  using chesskit::PromotablePieceType;
  using chesskit::SanNormalMove;
  using chesskit::Square;
  using enum chesskit::PieceType;
  using enum chesskit::File;
  using enum chesskit::Rank;
  using enum chesskit::CheckIndicator;

  EXPECT_EQ(std::format("{}",
                        SanNormalMove{
                            .pieceType = kPawn,
                            .destination = Square(kE, k4),
                        }),
            "e4");

  EXPECT_EQ(std::format("{}",
                        SanNormalMove{
                            .pieceType = kPawn,
                            .destination = Square(kA, k8),
                            .promotion = PromotablePieceType::kQueen,
                        }),
            "a8=Q");

  EXPECT_EQ(std::format("{}",
                        SanNormalMove{
                            .pieceType = kQueen,
                            .origin = PartialSquare(kD, std::nullopt),
                            .destination = Square(kE, k4),
                        }),
            "Qde4");

  EXPECT_EQ(std::format("{}",
                        SanNormalMove{
                            .pieceType = kKnight,
                            .origin = PartialSquare(std::nullopt, k3),
                            .destination = Square(kD, k5),
                        }),
            "N3d5");

  EXPECT_EQ(std::format("{}",
                        SanNormalMove{
                            .pieceType = kBishop,
                            .origin = PartialSquare(kA, k3),
                            .destination = Square(kF, k8),
                        }),
            "Ba3f8");

  EXPECT_EQ(std::format("{}",
                        SanNormalMove{
                            .pieceType = kRook,
                            .isCapture = true,
                            .destination = Square(kH, k5),
                            .check_indicator = kCheck,
                        }),
            "Rxh5+");

  EXPECT_EQ(std::format("{}",
                        SanNormalMove{
                            .pieceType = kKing,
                            .isCapture = true,
                            .destination = Square(kF, k7),
                            .check_indicator = kCheckmate,
                        }),
            "Kxf7#");
}

TEST(SanNormalMoveTest, PawnSymbolIsOptionalDuringParsing) {
  auto parsed_p = chesskit::parse<chesskit::SanNormalMove>("Pe4");
  auto parsed = chesskit::parse<chesskit::SanNormalMove>("e4");

  EXPECT_EQ(parsed_p, parsed);

  parsed_p = chesskit::parse<chesskit::SanNormalMove>("Pd3xe4#");
  parsed = chesskit::parse<chesskit::SanNormalMove>("d3xe4#");

  EXPECT_EQ(parsed_p, parsed);

  parsed_p = chesskit::parse<chesskit::SanNormalMove>("P3xe4+");
  parsed = chesskit::parse<chesskit::SanNormalMove>("3xe4+");

  EXPECT_EQ(parsed_p, parsed);
}

TEST(SanNormalMoveTest, ParseHandlesInvalidInputCorrectly) {
  EXPECT_EQ(chesskit::parse<chesskit::SanNormalMove>("O-O").error(),
            chesskit::ParseError::kInvalidFile);
  EXPECT_EQ(chesskit::parse<chesskit::SanNormalMove>("e4=K").error(),
            chesskit::ParseError::kInvalidPromotablePieceType);
  EXPECT_EQ(chesskit::parse<chesskit::SanNormalMove>("e4=").error(),
            chesskit::ParseError::kInvalidPromotablePieceType);
  EXPECT_EQ(chesskit::parse<chesskit::SanNormalMove>("=Q").error(),
            chesskit::ParseError::kInvalidFile);
  EXPECT_EQ(chesskit::parse<chesskit::SanNormalMove>("e4xe").error(),
            chesskit::ParseError::kInvalidRank);
  EXPECT_EQ(chesskit::parse<chesskit::SanNormalMove>("e4x").error(),
            chesskit::ParseError::kInvalidFile);
  EXPECT_EQ(chesskit::parse<chesskit::SanNormalMove>("").error(),
            chesskit::ParseError::kInvalidFile);
  EXPECT_EQ(chesskit::parse<chesskit::SanNormalMove>("e").error(),
            chesskit::ParseError::kInvalidFile);
  EXPECT_EQ(chesskit::parse<chesskit::SanNormalMove>("4").error(),
            chesskit::ParseError::kInvalidFile);
  EXPECT_EQ(chesskit::parse<chesskit::SanNormalMove>("?").error(),
            chesskit::ParseError::kInvalidFile);
  EXPECT_EQ(chesskit::parse<chesskit::SanNormalMove>("e?").error(),
            chesskit::ParseError::kInvalidFile);
  EXPECT_EQ(chesskit::parse<chesskit::SanNormalMove>("e4?").error(),
            chesskit::ParseError::kExpectingEndOfString);
}

// SanMove

TEST(SanMoveTest, DefaultConstructionResultsInValidSanNormalMove) {
  chesskit::SanMove san;
  std::visit(
      [](const auto& value) {
        using T = std::decay_t<decltype(value)>;
        if constexpr (std::is_same_v<T, chesskit::SanNormalMove>) {
          EXPECT_TRUE(
              magic_enum::enum_contains<chesskit::PieceType>(value.pieceType));
          EXPECT_EQ(value.origin.file, std::nullopt);
          EXPECT_EQ(value.origin.rank, std::nullopt);
          EXPECT_EQ(value.isCapture, false);
          EXPECT_EQ(chesskit::index(value.destination), 0);
          EXPECT_EQ(value.promotion, std::nullopt);
          EXPECT_EQ(value.check_indicator, std::nullopt);
        } else {
          FAIL() << "SanMove should default to SanNormalMove.";
        }
      },
      san);
}

TEST(SanMoveTest, RoundTripConversionIsSuccessful) {
  std::ranges::for_each(kTestSanMoves, [](const auto& san) {
    EXPECT_EQ(san, chesskit::parse<chesskit::SanMove>(std::format("{}", san)));
  });
}

TEST(SanMoveTest, FormatProducesNonEmptyStrings) {
  std::ranges::for_each(kTestSanMoves, [](const auto& san) {
    EXPECT_FALSE(std::format("{}", san).empty());
  });
}

TEST(SanMoveTest, ParseHandlesInvalidInputCorrectly) {
  EXPECT_EQ(chesskit::parse<chesskit::SanMove>("O-0").error(),
            chesskit::ParseError::kInvalidFile);
  EXPECT_EQ(chesskit::parse<chesskit::SanMove>("0-0").error(),
            chesskit::ParseError::kInvalidFile);
  EXPECT_EQ(chesskit::parse<chesskit::SanMove>("o-o").error(),
            chesskit::ParseError::kInvalidFile);
  EXPECT_EQ(chesskit::parse<chesskit::SanMove>("x").error(),
            chesskit::ParseError::kInvalidFile);
  EXPECT_EQ(chesskit::parse<chesskit::SanMove>("O-O-0").error(),
            chesskit::ParseError::kExpectingEndOfString);

  EXPECT_EQ(chesskit::parse<chesskit::SanMove>("e4=K").error(),
            chesskit::ParseError::kInvalidPromotablePieceType);
  EXPECT_EQ(chesskit::parse<chesskit::SanMove>("e4=").error(),
            chesskit::ParseError::kInvalidPromotablePieceType);
  EXPECT_EQ(chesskit::parse<chesskit::SanMove>("=Q").error(),
            chesskit::ParseError::kInvalidFile);
  EXPECT_EQ(chesskit::parse<chesskit::SanMove>("e4xe").error(),
            chesskit::ParseError::kInvalidRank);
  EXPECT_EQ(chesskit::parse<chesskit::SanMove>("e4x").error(),
            chesskit::ParseError::kInvalidFile);
  EXPECT_EQ(chesskit::parse<chesskit::SanMove>("").error(),
            chesskit::ParseError::kInvalidFile);
  EXPECT_EQ(chesskit::parse<chesskit::SanMove>("e").error(),
            chesskit::ParseError::kInvalidFile);
  EXPECT_EQ(chesskit::parse<chesskit::SanMove>("4").error(),
            chesskit::ParseError::kInvalidFile);
  EXPECT_EQ(chesskit::parse<chesskit::SanMove>("?").error(),
            chesskit::ParseError::kInvalidFile);
  EXPECT_EQ(chesskit::parse<chesskit::SanMove>("e?").error(),
            chesskit::ParseError::kInvalidFile);
  EXPECT_EQ(chesskit::parse<chesskit::SanMove>("e4?").error(),
            chesskit::ParseError::kExpectingEndOfString);
}
