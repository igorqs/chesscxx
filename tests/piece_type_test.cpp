#include <chesscxx/parse.h>
#include <chesscxx/parse_error.h>
#include <chesscxx/piece_type.h>
#include <gtest/gtest.h>

#include <format>
#include <magic_enum/magic_enum.hpp>
#include <magic_enum/magic_enum_utility.hpp>

TEST(PieceTypeCastingTest, ResultsInValidPiece) {
  magic_enum::enum_for_each<chesscxx::PromotablePieceType>(
      [](chesscxx::PromotablePieceType promotion) {
        EXPECT_TRUE(magic_enum::enum_contains<chesscxx::PieceType>(
            chesscxx::toPieceType(promotion)));
      });
}

TEST(PieceTypeCastingTest, UpperRoundTripConversionIsSuccessful) {
  magic_enum::enum_for_each<chesscxx::PromotablePieceType>(
      [](chesscxx::PromotablePieceType promotion) {
        EXPECT_EQ(promotion,
                  chesscxx::parse<chesscxx::PromotablePieceType>(
                      std::format("{:u}", chesscxx::toPieceType(promotion)),
                      chesscxx::parse_as::Uppercase{}));
      });
}

TEST(PieceTypeCastingTest, LowerRoundTripConversionIsSuccessful) {
  magic_enum::enum_for_each<chesscxx::PromotablePieceType>(
      [](chesscxx::PromotablePieceType promotion) {
        EXPECT_EQ(promotion,
                  chesscxx::parse<chesscxx::PromotablePieceType>(
                      std::format("{:l}", chesscxx::toPieceType(promotion)),
                      chesscxx::parse_as::Lowercase{}));
      });
}

// PieceType

TEST(PieceTypeTest, DefaultConstructionResultsInValidPieceType) {
  chesscxx::PieceType const piece_type{};
  EXPECT_TRUE(magic_enum::enum_contains<chesscxx::PieceType>(piece_type));
}

TEST(PieceTypeTest, UpperRoundTripConversionIsSuccessful) {
  magic_enum::enum_for_each<chesscxx::PieceType>(
      [](chesscxx::PieceType piece_type) {
        EXPECT_EQ(piece_type, chesscxx::parse<chesscxx::PieceType>(
                                  std::format("{:u}", piece_type),
                                  chesscxx::parse_as::Uppercase{}));
      });
}

TEST(PieceTypeTest, LowerRoundTripConversionIsSuccessful) {
  magic_enum::enum_for_each<chesscxx::PieceType>(
      [](chesscxx::PieceType piece_type) {
        EXPECT_EQ(piece_type, chesscxx::parse<chesscxx::PieceType>(
                                  std::format("{:l}", piece_type),
                                  chesscxx::parse_as::Lowercase{}));
      });
}

TEST(PieceTypeTest, FormatProducesExpectedOutput) {
  using chesscxx::PieceType;
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
  using chesscxx::parse_as::Uppercase;
  EXPECT_EQ(chesscxx::parse<chesscxx::PieceType>("k", Uppercase{}).error(),
            chesscxx::ParseError::kInvalidPieceType);
  EXPECT_EQ(chesscxx::parse<chesscxx::PieceType>("L", Uppercase{}).error(),
            chesscxx::ParseError::kInvalidPieceType);
  EXPECT_EQ(chesscxx::parse<chesscxx::PieceType>("K ", Uppercase{}).error(),
            chesscxx::ParseError::kExpectingEndOfString);
}

TEST(PieceTypeTest, ParseLowerHandlesInvalidInputCorrectly) {
  using chesscxx::parse_as::Lowercase;
  EXPECT_EQ(chesscxx::parse<chesscxx::PieceType>("K", Lowercase{}).error(),
            chesscxx::ParseError::kInvalidPieceType);
  EXPECT_EQ(chesscxx::parse<chesscxx::PieceType>("l", Lowercase{}).error(),
            chesscxx::ParseError::kInvalidPieceType);
  EXPECT_EQ(chesscxx::parse<chesscxx::PieceType>("k ", Lowercase{}).error(),
            chesscxx::ParseError::kExpectingEndOfString);
}

// PromotablePieceType

TEST(PromotablePieceTypeTest,
     DefaultConstructionResultsInValidPromotablePieceType) {
  chesscxx::PromotablePieceType const promotion{};
  EXPECT_TRUE(
      magic_enum::enum_contains<chesscxx::PromotablePieceType>(promotion));
}

TEST(PromotablePieceTypeTest, UpperRoundTripConversionIsSuccessful) {
  magic_enum::enum_for_each<chesscxx::PromotablePieceType>(
      [](chesscxx::PromotablePieceType promotion) {
        EXPECT_EQ(promotion, chesscxx::parse<chesscxx::PromotablePieceType>(
                                 std::format("{:u}", promotion),
                                 chesscxx::parse_as::Uppercase{}));
      });
}

TEST(PromotablePieceTypeTest, LowerRoundTripConversionIsSuccessful) {
  magic_enum::enum_for_each<chesscxx::PromotablePieceType>(
      [](chesscxx::PromotablePieceType promotion) {
        EXPECT_EQ(promotion, chesscxx::parse<chesscxx::PromotablePieceType>(
                                 std::format("{:l}", promotion),
                                 chesscxx::parse_as::Lowercase{}));
      });
}

TEST(PromotablePieceTypeTest, FormatProducesExpectedOutput) {
  using chesscxx::PromotablePieceType;
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
  using chesscxx::parse_as::Uppercase;
  EXPECT_EQ(
      chesscxx::parse<chesscxx::PromotablePieceType>("P", Uppercase{}).error(),
      chesscxx::ParseError::kInvalidPromotablePieceType);
  EXPECT_EQ(
      chesscxx::parse<chesscxx::PromotablePieceType>("K", Uppercase{}).error(),
      chesscxx::ParseError::kInvalidPromotablePieceType);
  EXPECT_EQ(
      chesscxx::parse<chesscxx::PromotablePieceType>("n", Uppercase{}).error(),
      chesscxx::ParseError::kInvalidPromotablePieceType);
  EXPECT_EQ(
      chesscxx::parse<chesscxx::PromotablePieceType>("L", Uppercase{}).error(),
      chesscxx::ParseError::kInvalidPromotablePieceType);
  EXPECT_EQ(
      chesscxx::parse<chesscxx::PromotablePieceType>("N ", Uppercase{}).error(),
      chesscxx::ParseError::kExpectingEndOfString);
}

TEST(PromotablePieceTypeTest, ParseLowerHandlesInvalidInputCorrectly) {
  using chesscxx::parse_as::Lowercase;
  EXPECT_EQ(
      chesscxx::parse<chesscxx::PromotablePieceType>("p", Lowercase{}).error(),
      chesscxx::ParseError::kInvalidPromotablePieceType);
  EXPECT_EQ(
      chesscxx::parse<chesscxx::PromotablePieceType>("k", Lowercase{}).error(),
      chesscxx::ParseError::kInvalidPromotablePieceType);
  EXPECT_EQ(
      chesscxx::parse<chesscxx::PromotablePieceType>("N", Lowercase{}).error(),
      chesscxx::ParseError::kInvalidPromotablePieceType);
  EXPECT_EQ(
      chesscxx::parse<chesscxx::PromotablePieceType>("l", Lowercase{}).error(),
      chesscxx::ParseError::kInvalidPromotablePieceType);
  EXPECT_EQ(
      chesscxx::parse<chesscxx::PromotablePieceType>("n ", Lowercase{}).error(),
      chesscxx::ParseError::kExpectingEndOfString);
}
