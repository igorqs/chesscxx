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
    auto ptr = ctx.begin();
    const auto end = ctx.end();

    auto is_end = [&end](const auto& ptr) { return ptr == end || *ptr == '}'; };

    if (is_end(ptr)) return ptr;

    if (*ptr == '?') {
      for (++ptr; !is_end(ptr); ++ptr) default_value.push_back(*ptr);
      return ptr;
    }

    for (; !is_end(ptr) && *ptr != '['; ++ptr) prefix.push_back(*ptr);
    if (is_end(ptr)) return ctx.begin();
    ++ptr;

    std::string underlying_specs;
    for (; !is_end(ptr) && *ptr != ']'; ++ptr) underlying_specs.push_back(*ptr);
    if (is_end(ptr)) return ctx.begin();
    ++ptr;
    ParseContext underlying_ctx(underlying_specs, underlying_specs.size());
    underlying_formatter.parse(underlying_ctx);

    for (; !is_end(ptr) && *ptr != '?'; ++ptr) suffix.push_back(*ptr);
    if (is_end(ptr)) return ptr;
    ++ptr;

    for (; !is_end(ptr); ++ptr) default_value.push_back(*ptr);

    return ptr;
  }

  template <typename FmtContext>
  auto format(const auto& opt, FmtContext& ctx) const {
    if (!opt.has_value()) return std::format_to(ctx.out(), "{}", default_value);

    auto out = ctx.out();
    out = std::format_to(out, "{}", prefix);
    ctx.advance_to(out);
    out = underlying_formatter.format(*opt, ctx);
    out = std::format_to(out, "{}", suffix);

    return out;
  }

  std::string default_value;
  std::string prefix;
  std::string suffix;
  std::formatter<T> underlying_formatter;
};

/// @}

#endif  // CHESSKIT_INCLUDE_CHESSKIT_BASE_FORMATTER_H_
