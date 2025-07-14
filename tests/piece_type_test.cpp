#include <chesskit/parse.h>
#include <chesskit/parse_error.h>
#include <chesskit/piece_type.h>
#include <gtest/gtest.h>

#include <format>
#include <magic_enum/magic_enum.hpp>
#include <magic_enum/magic_enum_utility.hpp>

TEST(PieceTypeCastingTest, ResultsInValidPiece) {
  magic_enum::enum_for_each<chesskit::PromotablePieceType>(
      [](chesskit::PromotablePieceType promotion) {
        EXPECT_TRUE(magic_enum::enum_contains<chesskit::PieceType>(
            chesskit::toPieceType(promotion)));
      });
}

TEST(PieceTypeCastingTest, UpperRoundTripConversionIsSuccessful) {
  magic_enum::enum_for_each<chesskit::PromotablePieceType>(
      [](chesskit::PromotablePieceType promotion) {
        EXPECT_EQ(promotion,
                  chesskit::parse<chesskit::PromotablePieceType>(
                      std::format("{:u}", chesskit::toPieceType(promotion)),
                      chesskit::parse_as::Uppercase{}));
      });
}

TEST(PieceTypeCastingTest, LowerRoundTripConversionIsSuccessful) {
  magic_enum::enum_for_each<chesskit::PromotablePieceType>(
      [](chesskit::PromotablePieceType promotion) {
        EXPECT_EQ(promotion,
                  chesskit::parse<chesskit::PromotablePieceType>(
                      std::format("{:l}", chesskit::toPieceType(promotion)),
                      chesskit::parse_as::Lowercase{}));
      });
}

// PieceType

TEST(PieceTypeTest, DefaultConstructionResultsInValidPieceType) {
  chesskit::PieceType const piece_type{};
  EXPECT_TRUE(magic_enum::enum_contains<chesskit::PieceType>(piece_type));
}

TEST(PieceTypeTest, UpperRoundTripConversionIsSuccessful) {
  magic_enum::enum_for_each<chesskit::PieceType>(
      [](chesskit::PieceType piece_type) {
        EXPECT_EQ(piece_type, chesskit::parse<chesskit::PieceType>(
                                  std::format("{:u}", piece_type),
                                  chesskit::parse_as::Uppercase{}));
      });
}

TEST(PieceTypeTest, LowerRoundTripConversionIsSuccessful) {
  magic_enum::enum_for_each<chesskit::PieceType>(
      [](chesskit::PieceType piece_type) {
        EXPECT_EQ(piece_type, chesskit::parse<chesskit::PieceType>(
                                  std::format("{:l}", piece_type),
                                  chesskit::parse_as::Lowercase{}));
      });
}

TEST(PieceTypeTest, FormatProducesExpectedOutput) {
  using chesskit::PieceType;
  EXPECT_EQ(std::format("{}", PieceType::kPawn), "pawn");
  EXPECT_EQ(std::format("{}", PieceType::kRook), "rook");
  EXPECT_EQ(std::format("{}", PieceType::kBishop), "bishop");
  EXPECT_EQ(std::format("{}", PieceType::kKnight), "knight");
  EXPECT_EQ(std::format("{}", PieceType::kQueen), "queen");
  EXPECT_EQ(std::format("{}", PieceType::kKing), "king");
  EXPECT_EQ(std::format("{:v}", PieceType::kPawn), "pawn");
  EXPECT_EQ(std::format("{:v}", PieceType::kRook), "rook");
  EXPECT_EQ(std::format("{:v}", PieceType::kBishop), "bishop");
  EXPECT_EQ(std::format("{:v}", PieceType::kKnight), "knight");
  EXPECT_EQ(std::format("{:v}", PieceType::kQueen), "queen");
  EXPECT_EQ(std::format("{:v}", PieceType::kKing), "king");
  EXPECT_EQ(std::format("{:u}", PieceType::kPawn), "P");
  EXPECT_EQ(std::format("{:u}", PieceType::kRook), "R");
  EXPECT_EQ(std::format("{:u}", PieceType::kBishop), "B");
  EXPECT_EQ(std::format("{:u}", PieceType::kKnight), "N");
  EXPECT_EQ(std::format("{:u}", PieceType::kQueen), "Q");
  EXPECT_EQ(std::format("{:u}", PieceType::kKing), "K");
  EXPECT_EQ(std::format("{:l}", PieceType::kPawn), "p");
  EXPECT_EQ(std::format("{:l}", PieceType::kRook), "r");
  EXPECT_EQ(std::format("{:l}", PieceType::kBishop), "b");
  EXPECT_EQ(std::format("{:l}", PieceType::kKnight), "n");
  EXPECT_EQ(std::format("{:l}", PieceType::kQueen), "q");
  EXPECT_EQ(std::format("{:l}", PieceType::kKing), "k");
}

TEST(PieceTypeTest, ParseUpperHandlesInvalidInputCorrectly) {
  using chesskit::parse_as::Uppercase;
  EXPECT_EQ(chesskit::parse<chesskit::PieceType>("k", Uppercase{}).error(),
            chesskit::ParseError::kInvalidPieceType);
  EXPECT_EQ(chesskit::parse<chesskit::PieceType>("L", Uppercase{}).error(),
            chesskit::ParseError::kInvalidPieceType);
  EXPECT_EQ(chesskit::parse<chesskit::PieceType>("K ", Uppercase{}).error(),
            chesskit::ParseError::kExpectingEndOfString);
}

TEST(PieceTypeTest, ParseLowerHandlesInvalidInputCorrectly) {
  using chesskit::parse_as::Lowercase;
  EXPECT_EQ(chesskit::parse<chesskit::PieceType>("K", Lowercase{}).error(),
            chesskit::ParseError::kInvalidPieceType);
  EXPECT_EQ(chesskit::parse<chesskit::PieceType>("l", Lowercase{}).error(),
            chesskit::ParseError::kInvalidPieceType);
  EXPECT_EQ(chesskit::parse<chesskit::PieceType>("k ", Lowercase{}).error(),
            chesskit::ParseError::kExpectingEndOfString);
}

// PromotablePieceType

TEST(PromotablePieceTypeTest,
     DefaultConstructionResultsInValidPromotablePieceType) {
  chesskit::PromotablePieceType const promotion{};
  EXPECT_TRUE(
      magic_enum::enum_contains<chesskit::PromotablePieceType>(promotion));
}

TEST(PromotablePieceTypeTest, UpperRoundTripConversionIsSuccessful) {
  magic_enum::enum_for_each<chesskit::PromotablePieceType>(
      [](chesskit::PromotablePieceType promotion) {
        EXPECT_EQ(promotion, chesskit::parse<chesskit::PromotablePieceType>(
                                 std::format("{:u}", promotion),
                                 chesskit::parse_as::Uppercase{}));
      });
}

TEST(PromotablePieceTypeTest, LowerRoundTripConversionIsSuccessful) {
  magic_enum::enum_for_each<chesskit::PromotablePieceType>(
      [](chesskit::PromotablePieceType promotion) {
        EXPECT_EQ(promotion, chesskit::parse<chesskit::PromotablePieceType>(
                                 std::format("{:l}", promotion),
                                 chesskit::parse_as::Lowercase{}));
      });
}

TEST(PromotablePieceTypeTest, FormatProducesExpectedOutput) {
  using chesskit::PromotablePieceType;
  EXPECT_EQ(std::format("{}", PromotablePieceType::kRook), "rook");
  EXPECT_EQ(std::format("{}", PromotablePieceType::kBishop), "bishop");
  EXPECT_EQ(std::format("{}", PromotablePieceType::kKnight), "knight");
  EXPECT_EQ(std::format("{}", PromotablePieceType::kQueen), "queen");
  EXPECT_EQ(std::format("{:v}", PromotablePieceType::kRook), "rook");
  EXPECT_EQ(std::format("{:v}", PromotablePieceType::kBishop), "bishop");
  EXPECT_EQ(std::format("{:v}", PromotablePieceType::kKnight), "knight");
  EXPECT_EQ(std::format("{:v}", PromotablePieceType::kQueen), "queen");
  EXPECT_EQ(std::format("{:u}", PromotablePieceType::kRook), "R");
  EXPECT_EQ(std::format("{:u}", PromotablePieceType::kBishop), "B");
  EXPECT_EQ(std::format("{:u}", PromotablePieceType::kKnight), "N");
  EXPECT_EQ(std::format("{:u}", PromotablePieceType::kQueen), "Q");
  EXPECT_EQ(std::format("{:l}", PromotablePieceType::kRook), "r");
  EXPECT_EQ(std::format("{:l}", PromotablePieceType::kBishop), "b");
  EXPECT_EQ(std::format("{:l}", PromotablePieceType::kKnight), "n");
  EXPECT_EQ(std::format("{:l}", PromotablePieceType::kQueen), "q");
}

TEST(PromotablePieceTypeTest, ParseUpperHandlesInvalidInputCorrectly) {
  using chesskit::parse_as::Uppercase;
  EXPECT_EQ(
      chesskit::parse<chesskit::PromotablePieceType>("P", Uppercase{}).error(),
      chesskit::ParseError::kInvalidPromotablePieceType);
  EXPECT_EQ(
      chesskit::parse<chesskit::PromotablePieceType>("K", Uppercase{}).error(),
      chesskit::ParseError::kInvalidPromotablePieceType);
  EXPECT_EQ(
      chesskit::parse<chesskit::PromotablePieceType>("n", Uppercase{}).error(),
      chesskit::ParseError::kInvalidPromotablePieceType);
  EXPECT_EQ(
      chesskit::parse<chesskit::PromotablePieceType>("L", Uppercase{}).error(),
      chesskit::ParseError::kInvalidPromotablePieceType);
  EXPECT_EQ(
      chesskit::parse<chesskit::PromotablePieceType>("N ", Uppercase{}).error(),
      chesskit::ParseError::kExpectingEndOfString);
}

TEST(PromotablePieceTypeTest, ParseLowerHandlesInvalidInputCorrectly) {
  using chesskit::parse_as::Lowercase;
  EXPECT_EQ(
      chesskit::parse<chesskit::PromotablePieceType>("p", Lowercase{}).error(),
      chesskit::ParseError::kInvalidPromotablePieceType);
  EXPECT_EQ(
      chesskit::parse<chesskit::PromotablePieceType>("k", Lowercase{}).error(),
      chesskit::ParseError::kInvalidPromotablePieceType);
  EXPECT_EQ(
      chesskit::parse<chesskit::PromotablePieceType>("N", Lowercase{}).error(),
      chesskit::ParseError::kInvalidPromotablePieceType);
  EXPECT_EQ(
      chesskit::parse<chesskit::PromotablePieceType>("l", Lowercase{}).error(),
      chesskit::ParseError::kInvalidPromotablePieceType);
  EXPECT_EQ(
      chesskit::parse<chesskit::PromotablePieceType>("n ", Lowercase{}).error(),
      chesskit::ParseError::kExpectingEndOfString);
}
