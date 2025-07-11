#include <chesskit/formatter/base_formatter.h>
#include <gtest/gtest.h>

#include <format>
#include <optional>
#include <utility>

struct Point {
  int x, y;
};

template <>
struct std::formatter<Point> : chesskit::internal::BaseFormatter {
  bool compact = false;

  constexpr auto parse(auto& ctx) {
    auto it = ctx.begin();
    if (it == ctx.end()) return it;
    if (*it == 'c') {
      compact = true;
      ++it;
    }
    if (it != ctx.end() && *it != '}')
      throw std::format_error("Invalid format args for Point.");

    return it;
  }

  auto format(const Point& p, auto& ctx) const {
    if (compact) {
      return std::format_to(ctx.out(), "{}:{}", p.x, p.y);
    } else {
      return std::format_to(ctx.out(), "({}, {})", p.x, p.y);
    }
  }
};

TEST(OptionalFormatter, FormatNonNullAndValidFmtStringCorrectly) {
  std::optional<Point> p = Point(3, 14);
  EXPECT_EQ(std::format("{}", p), "(3, 14)");
  EXPECT_EQ(std::format("{:[c]}", p), "3:14");
  EXPECT_EQ(std::format("{:?}", p), "(3, 14)");
  EXPECT_EQ(std::format("{:??}", p), "(3, 14)");
  EXPECT_EQ(std::format("{:?[]}", p), "(3, 14)");
  EXPECT_EQ(std::format("{:[]?}", p), "(3, 14)");
  EXPECT_EQ(std::format("{:[][]}", p), "(3, 14)[]");
  EXPECT_EQ(std::format("{:[][][]}", p), "(3, 14)[][]");
  EXPECT_EQ(std::format("{:?foo}", p), "(3, 14)");
  EXPECT_EQ(std::format("{:[c]?foo}", p), "3:14");
  EXPECT_EQ(std::format("{:bar[]}", p), "bar(3, 14)");
  EXPECT_EQ(std::format("{:bar[]?foo}", p), "bar(3, 14)");
  EXPECT_EQ(std::format("{:bar[c]}", p), "bar3:14");
  EXPECT_EQ(std::format("{:bar[c]?foo}", p), "bar3:14");
  EXPECT_EQ(std::format("{:[]baz}", p), "(3, 14)baz");
  EXPECT_EQ(std::format("{:[]baz?foo}", p), "(3, 14)baz");
  EXPECT_EQ(std::format("{:[c]baz}", p), "3:14baz");
  EXPECT_EQ(std::format("{:[c]baz?foo}", p), "3:14baz");
  EXPECT_EQ(std::format("{:bar[]baz}", p), "bar(3, 14)baz");
  EXPECT_EQ(std::format("{:bar[c]baz}", p), "bar3:14baz");
  EXPECT_EQ(std::format("{:bar[]baz?foo}", p), "bar(3, 14)baz");
  EXPECT_EQ(std::format("{:bar[c]baz?foo}", p), "bar3:14baz");
}

TEST(OptionalFormatter, FormatNullAndValidFmtStringCorrectly) {
  std::optional<Point> p = std::nullopt;
  EXPECT_EQ(std::format("{}", p), "");
  EXPECT_EQ(std::format("{:[c]}", p), "");
  EXPECT_EQ(std::format("{:?}", p), "");
  EXPECT_EQ(std::format("{:??}", p), "?");
  EXPECT_EQ(std::format("{:?[]}", p), "[]");
  EXPECT_EQ(std::format("{:[]?}", p), "");
  EXPECT_EQ(std::format("{:[][]}", p), "");
  EXPECT_EQ(std::format("{:[][][]}", p), "");
  EXPECT_EQ(std::format("{:?foo}", p), "foo");
  EXPECT_EQ(std::format("{:[c]?foo}", p), "foo");
  EXPECT_EQ(std::format("{:bar[]}", p), "");
  EXPECT_EQ(std::format("{:bar[]?foo}", p), "foo");
  EXPECT_EQ(std::format("{:bar[c]}", p), "");
  EXPECT_EQ(std::format("{:bar[c]?foo}", p), "foo");
  EXPECT_EQ(std::format("{:[]baz}", p), "");
  EXPECT_EQ(std::format("{:[]baz?foo}", p), "foo");
  EXPECT_EQ(std::format("{:[c]baz}", p), "");
  EXPECT_EQ(std::format("{:[c]baz?foo}", p), "foo");
  EXPECT_EQ(std::format("{:bar[]baz}", p), "");
  EXPECT_EQ(std::format("{:bar[c]baz}", p), "");
  EXPECT_EQ(std::format("{:bar[]baz?foo}", p), "foo");
  EXPECT_EQ(std::format("{:bar[c]baz?foo}", p), "foo");
}

TEST(OptionalFormatter, ThrowOnInvalidFmtString) {
  std::optional<Point> p;
  auto args = std::make_format_args(p);

  EXPECT_THROW(std::ignore = std::vformat("{:[d]}", args), std::format_error);
  EXPECT_THROW(std::ignore = std::vformat("{:[car]}", args), std::format_error);
  EXPECT_THROW(std::ignore = std::vformat("{:[:d]}", args), std::format_error);
  EXPECT_THROW(std::ignore = std::vformat("{:[}", args), std::format_error);
  EXPECT_THROW(std::ignore = std::vformat("{:[bar}", args), std::format_error);
  EXPECT_THROW(std::ignore = std::vformat("{:]}", args), std::format_error);
  EXPECT_THROW(std::ignore = std::vformat("{:baz]}", args), std::format_error);
}
