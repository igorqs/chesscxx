#include <chesskit/castling_rights.h>
#include <chesskit/castling_side.h>
#include <chesskit/color.h>
#include <chesskit/parse.h>
#include <chesskit/parse_error.h>
#include <gtest/gtest.h>

#include <algorithm>
#include <bitset>
#include <cstddef>
#include <format>
#include <functional>
#include <magic_enum/magic_enum_utility.hpp>
#include <ranges>
#include <unordered_set>

constexpr auto kAll = chesskit::CastlingRights{};
constexpr auto kNone = chesskit::CastlingRights{0};

static constexpr int kMaxBitset =
    (1 << chesskit::CastlingRights::kNumCastlingRights);

constexpr static auto kAllCastlingRights =
    std::views::iota(0, kMaxBitset) | std::views::transform([](auto bits) {
      return chesskit::CastlingRights(bits);
    });

TEST(CastlingRightsTest, DefaultConstructionInitializeWithAllCastlingEnabled) {
  chesskit::CastlingRights const rights;
  EXPECT_TRUE(rights.all());
}

TEST(CastlingRightsTest, BitsetConstructionInitializeRawBitsCorrectly) {
  std::ranges::for_each(std::views::iota(0, kMaxBitset), [](auto bits) {
    EXPECT_EQ(chesskit::CastlingRights(bits).toBitset(), bits);
  });
}

TEST(CastlingRightsTest, ComparesEqualValuesSuccessfully) {
  std::ranges::for_each(kAllCastlingRights,
                        [](const auto& rights) { EXPECT_EQ(rights, rights); });
}

TEST(CastlingRightsTest, ComparesDifferentValuesSuccessfully) {
  constexpr auto kSize = static_cast<int>(kAllCastlingRights.size());
  for (int const lhs : std::views::iota(0, kSize)) {
    for (int const rhs : std::views::iota(0, kSize)) {
      if (lhs != rhs) {
        EXPECT_NE(kAllCastlingRights[lhs], kAllCastlingRights[rhs]);
      }
    }
  }
}

TEST(CastlingRightsTest, CanCastleMatchesBitset) {
  using magic_enum::enum_for_each;

  std::ranges::for_each(kAllCastlingRights, [](auto rights) {
    std::bitset<chesskit::CastlingRights::kNumCastlingRights> bits;
    int bit_index = 0;

    enum_for_each<chesskit::Color>([&](chesskit::Color color) {
      enum_for_each<chesskit::CastlingSide>([&](chesskit::CastlingSide side) {
        bits.set(bit_index++, rights.canCastle(side, color));
      });
    });

    EXPECT_EQ(bits, rights.toBitset());
  });
}

TEST(CastlingRightsTest, AllReturnsTrueOnlyWhenAllRightsSet) {
  std::ranges::for_each(kAllCastlingRights, [](const auto& rights) {
    EXPECT_EQ(rights.all(), rights == kAll);
  });
}

TEST(CastlingRightsTest, AnyReturnsFalseOnlyWhenNoRightsSet) {
  std::ranges::for_each(kAllCastlingRights, [](const auto& rights) {
    EXPECT_EQ(!rights.any(), rights == kNone);
  });
}

TEST(CastlingRightsTest, NoneReturnsTrueOnlyWhenNoRightsSet) {
  std::ranges::for_each(kAllCastlingRights, [](const auto& rights) {
    EXPECT_EQ(rights.none(), rights == kNone);
  });
}

TEST(CastlingRightsTest, EnableHandlesBySideAndColorCorrectly) {
  using magic_enum::enum_for_each;

  enum_for_each<chesskit::Color>([&](chesskit::Color enabled_color) {
    enum_for_each<chesskit::CastlingSide>(
        [&](chesskit::CastlingSide enabled_side) {
          auto rights = kNone;
          rights.enable(enabled_side, enabled_color);

          enum_for_each<chesskit::Color>([&](chesskit::Color test_color) {
            enum_for_each<chesskit::CastlingSide>(
                [&](chesskit::CastlingSide test_side) {
                  bool const expected = (test_color == enabled_color &&
                                         test_side == enabled_side);
                  EXPECT_EQ(rights.canCastle(test_side, test_color), expected);
                });
          });
        });
  });
}

TEST(CastlingRightsTest, EnableHandlesByColorCorrectly) {
  using magic_enum::enum_for_each;

  enum_for_each<chesskit::Color>([&](chesskit::Color enabled_color) {
    auto rights = kNone;
    rights.enable(enabled_color);

    enum_for_each<chesskit::Color>([&](chesskit::Color test_color) {
      enum_for_each<chesskit::CastlingSide>(
          [&](chesskit::CastlingSide test_side) {
            bool const expected = (test_color == enabled_color);
            EXPECT_EQ(rights.canCastle(test_side, test_color), expected);
          });
    });
  });
}

TEST(CastlingRightsTest, EnableHandlesAllCorrectly) {
  auto rights = kNone;
  rights.enable();
  EXPECT_TRUE(rights.all());
}

TEST(CastlingRightsTest, DisableHandlesBySideAndColorCorrectly) {
  using magic_enum::enum_for_each;

  enum_for_each<chesskit::Color>([&](chesskit::Color disabled_color) {
    enum_for_each<chesskit::CastlingSide>(
        [&](chesskit::CastlingSide disabled_side) {
          auto rights = kAll;
          rights.disable(disabled_side, disabled_color);

          enum_for_each<chesskit::Color>([&](chesskit::Color test_color) {
            enum_for_each<chesskit::CastlingSide>(
                [&](chesskit::CastlingSide test_side) {
                  bool const expected = (test_color != disabled_color ||
                                         test_side != disabled_side);
                  EXPECT_EQ(rights.canCastle(test_side, test_color), expected);
                });
          });
        });
  });
}

TEST(CastlingRightsTest, DisableHandlesByColorCorrectly) {
  using magic_enum::enum_for_each;

  enum_for_each<chesskit::Color>([&](chesskit::Color disabled_color) {
    auto rights = kAll;
    rights.disable(disabled_color);

    enum_for_each<chesskit::Color>([&](chesskit::Color test_color) {
      enum_for_each<chesskit::CastlingSide>(
          [&](chesskit::CastlingSide test_side) {
            bool const expected = (test_color != disabled_color);
            EXPECT_EQ(rights.canCastle(test_side, test_color), expected);
          });
    });
  });
}

TEST(CastlingRightsTest, DisableHandlesAllCorrectly) {
  auto rights = kAll;
  rights.disable();
  EXPECT_TRUE(rights.none());
}

TEST(CastlingRightsTest, HashProducesUniqueValues) {
  std::unordered_set<size_t> hashes;

  std::ranges::for_each(kAllCastlingRights, [&](const auto& rights) {
    EXPECT_TRUE(
        hashes.insert(std::hash<chesskit::CastlingRights>{}(rights)).second);
  });
}

TEST(CastlingRightsTest, RoundTripConversionIsSuccessful) {
  std::ranges::for_each(kAllCastlingRights, [](const auto& rights) {
    EXPECT_EQ(rights, chesskit::parse<chesskit::CastlingRights>(
                          std::format("{}", rights)));
  });
}

TEST(CastlingRightsTest, FormatProducesNonEmptyStrings) {
  std::ranges::for_each(kAllCastlingRights, [](const auto& rights) {
    EXPECT_FALSE(std::format("{}", rights).empty());
  });
}

TEST(CastlingRightsTest, FormatProducesExpectedOutput) {
  using chesskit::CastlingRights;
  EXPECT_EQ(std::format("{}", kNone), "-");
  EXPECT_EQ(std::format("{}", kAll), "KQkq");
  EXPECT_EQ(std::format("{}", CastlingRights(0b0111)), "KQk");
  EXPECT_EQ(std::format("{}", CastlingRights(0b0011)), "KQ");
  EXPECT_EQ(std::format("{}", CastlingRights(0b0001)), "K");
  EXPECT_EQ(std::format("{}", CastlingRights(0b0010)), "Q");
  EXPECT_EQ(std::format("{}", CastlingRights(0b0100)), "k");
  EXPECT_EQ(std::format("{}", CastlingRights(0b1000)), "q");
}

TEST(CastlingRightsTest, ParseHandlesInvalidInputCorrectly) {
  EXPECT_EQ(chesskit::parse<chesskit::CastlingRights>("x").error(),
            chesskit::ParseError::kInvalidCastlingAvailability);
  EXPECT_EQ(chesskit::parse<chesskit::CastlingRights>("").error(),
            chesskit::ParseError::kInvalidCastlingAvailability);
  EXPECT_EQ(chesskit::parse<chesskit::CastlingRights>("qkQK").error(),
            chesskit::ParseError::kExpectingEndOfString);
  EXPECT_EQ(chesskit::parse<chesskit::CastlingRights>("QQQ").error(),
            chesskit::ParseError::kExpectingEndOfString);
}
