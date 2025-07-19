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
    auto ptr = ctx.begin();
    if (ptr == ctx.end()) return ptr;
    if (*ptr == 'c') {
      compact = true;
      ++ptr;
    }
    if (ptr != ctx.end() && *ptr != '}') {
      throw std::format_error("Invalid format args for Point.");
    }

    return ptr;
  }

  auto format(const Point& point, auto& ctx) const {
    if (compact) {
      return std::format_to(ctx.out(), "{}:{}", point.x, point.y);
    }
    return std::format_to(ctx.out(), "({}, {})", point.x, point.y);
  }
};

TEST(OptionalFormatter, FormatNonNullAndValidFmtStringCorrectly) {
  const std::optional<Point> point = Point(3, 14);
  EXPECT_EQ(std::format("{}", point), "(3, 14)");
  EXPECT_EQ(std::format("{:[c]}", point), "3:14");
  EXPECT_EQ(std::format("{:?}", point), "(3, 14)");
  EXPECT_EQ(std::format("{:??}", point), "(3, 14)");
  EXPECT_EQ(std::format("{:?[]}", point), "(3, 14)");
  EXPECT_EQ(std::format("{:[]?}", point), "(3, 14)");
  EXPECT_EQ(std::format("{:[][]}", point), "(3, 14)[]");
  EXPECT_EQ(std::format("{:[][][]}", point), "(3, 14)[][]");
  EXPECT_EQ(std::format("{:?foo}", point), "(3, 14)");
  EXPECT_EQ(std::format("{:[c]?foo}", point), "3:14");
  EXPECT_EQ(std::format("{:bar[]}", point), "bar(3, 14)");
  EXPECT_EQ(std::format("{:bar[]?foo}", point), "bar(3, 14)");
  EXPECT_EQ(std::format("{:bar[c]}", point), "bar3:14");
  EXPECT_EQ(std::format("{:bar[c]?foo}", point), "bar3:14");
  EXPECT_EQ(std::format("{:[]baz}", point), "(3, 14)baz");
  EXPECT_EQ(std::format("{:[]baz?foo}", point), "(3, 14)baz");
  EXPECT_EQ(std::format("{:[c]baz}", point), "3:14baz");
  EXPECT_EQ(std::format("{:[c]baz?foo}", point), "3:14baz");
  EXPECT_EQ(std::format("{:bar[]baz}", point), "bar(3, 14)baz");
  EXPECT_EQ(std::format("{:bar[c]baz}", point), "bar3:14baz");
  EXPECT_EQ(std::format("{:bar[]baz?foo}", point), "bar(3, 14)baz");
  EXPECT_EQ(std::format("{:bar[c]baz?foo}", point), "bar3:14baz");
}

TEST(OptionalFormatter, FormatNullAndValidFmtStringCorrectly) {
  std::optional<Point> point = std::nullopt;
  EXPECT_EQ(std::format("{}", point), "");
  EXPECT_EQ(std::format("{:[c]}", point), "");
  EXPECT_EQ(std::format("{:?}", point), "");
  EXPECT_EQ(std::format("{:??}", point), "?");
  EXPECT_EQ(std::format("{:?[]}", point), "[]");
  EXPECT_EQ(std::format("{:[]?}", point), "");
  EXPECT_EQ(std::format("{:[][]}", point), "");
  EXPECT_EQ(std::format("{:[][][]}", point), "");
  EXPECT_EQ(std::format("{:?foo}", point), "foo");
  EXPECT_EQ(std::format("{:[c]?foo}", point), "foo");
  EXPECT_EQ(std::format("{:bar[]}", point), "");
  EXPECT_EQ(std::format("{:bar[]?foo}", point), "foo");
  EXPECT_EQ(std::format("{:bar[c]}", point), "");
  EXPECT_EQ(std::format("{:bar[c]?foo}", point), "foo");
  EXPECT_EQ(std::format("{:[]baz}", point), "");
  EXPECT_EQ(std::format("{:[]baz?foo}", point), "foo");
  EXPECT_EQ(std::format("{:[c]baz}", point), "");
  EXPECT_EQ(std::format("{:[c]baz?foo}", point), "foo");
  EXPECT_EQ(std::format("{:bar[]baz}", point), "");
  EXPECT_EQ(std::format("{:bar[c]baz}", point), "");
  EXPECT_EQ(std::format("{:bar[]baz?foo}", point), "foo");
  EXPECT_EQ(std::format("{:bar[c]baz?foo}", point), "foo");
}

TEST(OptionalFormatter, ThrowOnInvalidFmtString) {
  std::optional<Point> point;
  auto args = std::make_format_args(point);

  EXPECT_THROW(std::ignore = std::vformat("{:[d]}", args), std::format_error);
  EXPECT_THROW(std::ignore = std::vformat("{:[car]}", args), std::format_error);
  EXPECT_THROW(std::ignore = std::vformat("{:[:d]}", args), std::format_error);
  EXPECT_THROW(std::ignore = std::vformat("{:[}", args), std::format_error);
  EXPECT_THROW(std::ignore = std::vformat("{:[bar}", args), std::format_error);
  EXPECT_THROW(std::ignore = std::vformat("{:]}", args), std::format_error);
  EXPECT_THROW(std::ignore = std::vformat("{:baz]}", args), std::format_error);
}
