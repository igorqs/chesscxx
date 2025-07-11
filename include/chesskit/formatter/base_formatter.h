#ifndef CHESSKIT_INCLUDE_CHESSKIT_BASE_FORMATTER_H_
#define CHESSKIT_INCLUDE_CHESSKIT_BASE_FORMATTER_H_

#include <format>
#include <optional>

namespace chesskit::internal {
struct BaseFormatter {};
}  // namespace chesskit::internal

/// @defgroup OptionalFormatter Optional formatter
/// @{

/// @brief Formatter for `std::optional<T>` where `T` is a `chesskit` type that
/// supports formatting.
///
/// The format string can optionally include a prefix, suffix, underlying
/// formatting specifications for the contained type, and a default value to
/// display when the optional does not contain a value.
///
/// Format string syntax:
/// ```
///   prefix[underlying_specs]suffix?default_value
/// ```
/// - `prefix`: Text inserted before the value
/// - `[underlying_specs]`: Format specs passed to the formatter of the
/// underlying `T`
/// - `suffix`: Text inserted after the value
/// - `?default_value`: Fallback value if the optional is empty
///
/// @tparam T A `chesskit` type with `std::formatter<T>` defined.
template <typename T>
  requires std::is_base_of_v<chesskit::internal::BaseFormatter,
                             std::formatter<T>>
struct std::formatter<std::optional<T>> {
  template <typename ParseContext>
  constexpr auto parse(ParseContext& ctx) {
    auto it = ctx.begin();
    const auto end = ctx.end();

    auto isEnd = [&end](const auto& it) { return it == end || *it == '}'; };

    if (isEnd(it)) return it;

    if (*it == '?') {
      for (++it; !isEnd(it); ++it) defaultValue.push_back(*it);
      return it;
    }

    for (; !isEnd(it) && *it != '['; ++it) prefix.push_back(*it);
    if (isEnd(it)) return ctx.begin();
    ++it;

    std::string underlyingSpecs;
    for (; !isEnd(it) && *it != ']'; ++it) underlyingSpecs.push_back(*it);
    if (isEnd(it)) return ctx.begin();
    ++it;
    ParseContext underlyingCtx(underlyingSpecs.data(), underlyingSpecs.size());
    underlyingFormatter.parse(underlyingCtx);

    for (; !isEnd(it) && *it != '?'; ++it) suffix.push_back(*it);
    if (isEnd(it)) return it;
    ++it;

    for (; !isEnd(it); ++it) defaultValue.push_back(*it);

    return it;
  }

  template <typename FmtContext>
  auto format(const auto& opt, FmtContext& ctx) const {
    if (!opt.has_value()) return std::format_to(ctx.out(), "{}", defaultValue);

    auto out = ctx.out();
    out = std::format_to(out, "{}", prefix);
    ctx.advance_to(out);
    out = underlyingFormatter.format(*opt, ctx);
    out = std::format_to(out, "{}", suffix);

    return out;
  }

  std::string defaultValue;
  std::string prefix;
  std::string suffix;
  std::formatter<T> underlyingFormatter;
};

/// @}

#endif  // CHESSKIT_INCLUDE_CHESSKIT_BASE_FORMATTER_H_
