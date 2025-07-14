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
  for (int const i : std::views::iota(0, kSize)) {
    for (int const j : std::views::iota(0, kSize)) {
      if (i != j) EXPECT_NE(kAllCastlingRights[i], kAllCastlingRights[j]);
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

  enum_for_each<chesskit::Color>([&](chesskit::Color color) {
    enum_for_each<chesskit::CastlingSide>([&](chesskit::CastlingSide side) {
      auto rights = kNone;
      rights.enable(side, color);

      enum_for_each<chesskit::Color>([&](chesskit::Color c) {
        enum_for_each<chesskit::CastlingSide>([&](chesskit::CastlingSide s) {
          bool const expected = (c == color && s == side);
          EXPECT_EQ(rights.canCastle(s, c), expected);
        });
      });
    });
  });
}

TEST(CastlingRightsTest, EnableHandlesByColorCorrectly) {
  using magic_enum::enum_for_each;

  enum_for_each<chesskit::Color>([&](chesskit::Color color) {
    auto rights = kNone;
    rights.enable(color);

    enum_for_each<chesskit::Color>([&](chesskit::Color c) {
      enum_for_each<chesskit::CastlingSide>([&](chesskit::CastlingSide s) {
        bool const expected = (c == color);
        EXPECT_EQ(rights.canCastle(s, c), expected);
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

  enum_for_each<chesskit::Color>([&](chesskit::Color color) {
    enum_for_each<chesskit::CastlingSide>([&](chesskit::CastlingSide side) {
      auto rights = kAll;
      rights.disable(side, color);

      enum_for_each<chesskit::Color>([&](chesskit::Color c) {
        enum_for_each<chesskit::CastlingSide>([&](chesskit::CastlingSide s) {
          bool const expected = (c != color || s != side);
          EXPECT_EQ(rights.canCastle(s, c), expected);
        });
      });
    });
  });
}

TEST(CastlingRightsTest, DisableHandlesByColorCorrectly) {
  using magic_enum::enum_for_each;

  enum_for_each<chesskit::Color>([&](chesskit::Color color) {
    auto rights = kAll;
    rights.disable(color);

    enum_for_each<chesskit::Color>([&](chesskit::Color c) {
      enum_for_each<chesskit::CastlingSide>([&](chesskit::CastlingSide s) {
        bool const expected = (c != color);
        EXPECT_EQ(rights.canCastle(s, c), expected);
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
