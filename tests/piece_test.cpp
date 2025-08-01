#include <chesscxx/color.h>
#include <chesscxx/parse.h>
#include <chesscxx/parse_error.h>
#include <chesscxx/piece.h>
#include <chesscxx/piece_type.h>
#include <gtest/gtest.h>

#include <algorithm>
#include <array>
#include <cstddef>
#include <format>
#include <functional>
#include <magic_enum/magic_enum.hpp>
#include <ranges>
#include <string_view>
#include <tuple>
#include <unordered_set>

constexpr auto kAllPieces = std::views::cartesian_product(
                                magic_enum::enum_values<chesscxx::PieceType>(),
                                magic_enum::enum_values<chesscxx::Color>()) |
                            std::views::transform([](const auto& product) {
                              const auto& [piece_type, color] = product;
                              return chesscxx::Piece(piece_type, color);
                            });

TEST(PieceTest, DefaultConstructionResultsInValidPiece) {
  chesscxx::Piece const piece;
  EXPECT_TRUE(magic_enum::enum_contains<chesscxx::PieceType>(piece.type));
  EXPECT_TRUE(magic_enum::enum_contains<chesscxx::Color>(piece.color));
}

TEST(PieceTest, ComparesEqualValuesSuccessfully) {
  std::ranges::for_each(kAllPieces,
                        [](const auto& piece) { EXPECT_EQ(piece, piece); });
}

TEST(PieceTest, ComparesDifferentValuesSuccessfully) {
  constexpr auto kSize = static_cast<int>(kAllPieces.size());

  for (int const lhs : std::views::iota(0, kSize)) {
    for (int const rhs : std::views::iota(0, kSize)) {
      if (lhs != rhs) {
        EXPECT_NE(kAllPieces[lhs], kAllPieces[rhs]);
      }
    }
  }
}

TEST(PieceTest, HashProducesUniqueValues) {
  std::unordered_set<size_t> hashes;

  std::ranges::for_each(kAllPieces, [&](const auto& piece) {
    EXPECT_TRUE(hashes.insert(std::hash<chesscxx::Piece>{}(piece)).second);
  });
}

TEST(PieceTest, RoundTripConversionIsSuccessful) {
  std::ranges::for_each(kAllPieces, [](const auto& piece) {
    EXPECT_EQ(piece,
              chesscxx::parse<chesscxx::Piece>(std::format("{:c}", piece)));
  });
}

TEST(PieceTest, FormatProducesExpectedOutput) {
  using chesscxx::Color;
  using chesscxx::Piece;
  using chesscxx::PieceType;

  constexpr std::array<std::tuple<Piece, std::string_view, std::string_view>,
                       kAllPieces.size()>
      kFixtures = {{
          {Piece(PieceType::kPawn, Color::kWhite), "P", "white pawn"},
          {Piece(PieceType::kPawn, Color::kBlack), "p", "black pawn"},
          {Piece(PieceType::kRook, Color::kWhite), "R", "white rook"},
          {Piece(PieceType::kRook, Color::kBlack), "r", "black rook"},
          {Piece(PieceType::kBishop, Color::kWhite), "B", "white bishop"},
          {Piece(PieceType::kBishop, Color::kBlack), "b", "black bishop"},
          {Piece(PieceType::kKnight, Color::kWhite), "N", "white knight"},
          {Piece(PieceType::kKnight, Color::kBlack), "n", "black knight"},
          {Piece(PieceType::kQueen, Color::kWhite), "Q", "white queen"},
          {Piece(PieceType::kQueen, Color::kBlack), "q", "black queen"},
          {Piece(PieceType::kKing, Color::kWhite), "K", "white king"},
          {Piece(PieceType::kKing, Color::kBlack), "k", "black king"},
      }};

  for (const auto& [piece, compact, verbose] : kFixtures) {
    EXPECT_EQ(std::format("{:c}", piece), compact);
    EXPECT_EQ(std::format("{:v}", piece), verbose);
    EXPECT_EQ(std::format("{}", piece), verbose);
  }
}

TEST(PieceTest, ParseHandlesInvalidInputCorrectly) {
  EXPECT_EQ(chesscxx::parse<chesscxx::Piece>("X").error(),
            chesscxx::ParseError::kInvalidPiece);
  EXPECT_EQ(chesscxx::parse<chesscxx::Piece>("x").error(),
            chesscxx::ParseError::kInvalidPiece);
  EXPECT_EQ(chesscxx::parse<chesscxx::Piece>("q ").error(),
            chesscxx::ParseError::kExpectingEndOfString);
  EXPECT_EQ(chesscxx::parse<chesscxx::Piece>("Q ").error(),
            chesscxx::ParseError::kExpectingEndOfString);
}
