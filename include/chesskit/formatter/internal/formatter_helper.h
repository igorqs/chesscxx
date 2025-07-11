#ifndef CHESSKIT_INCLUDE_CHESSKIT_FORMATTER_INTERNAL_FORMATTER_HELPER_H_
#define CHESSKIT_INCLUDE_CHESSKIT_FORMATTER_INTERNAL_FORMATTER_HELPER_H_

#include <string_view>
#include <variant>

#include "../base_formatter.h"

namespace chesskit::internal {

struct RepetitionSpec {
  static constexpr std::string_view token = "rep";
};
struct CompactUpperSpec {
  static constexpr std::string_view token = "u";
};
struct CompactLowerSpec {
  static constexpr std::string_view token = "l";
};
struct CompactSpec {
  static constexpr std::string_view token = "c";
};
struct VerboseSpec {
  static constexpr std::string_view token = "v";
};
struct PgnSpec {
  static constexpr std::string_view token = "pgn";
};
struct FenSpec {
  static constexpr std::string_view token = "fen";
};
struct AsciiSpec {
  static constexpr std::string_view token = "ascii";
};
struct PieceListSpec {
  static constexpr std::string_view token = "lists";
};

struct NoSpec : BaseFormatter {
  template <typename ParseContext>
  constexpr auto parse(ParseContext& ctx) {
    return ctx.begin();
  }
};

template <typename... Specs>
  requires(sizeof...(Specs) > 0)
struct SpecDispatcher : BaseFormatter {
  template <typename ParseContext>
  constexpr auto parse(ParseContext& ctx) {
    auto it = ctx.begin();
    const auto end = ctx.end();

    std::string_view sv(ctx);

    ([&] {
      if (sv.size() < Specs::token.size()) return false;

      auto next = it + Specs::token.size();
      if (sv.starts_with(Specs::token) && ((next == end) || *next == '}')) {
        spec = Specs{};
        it = next;
        return true;
      }
      return false;
    }() ||
     ...);

    return it;
  }

  template <typename Self, typename FmtContext>
  auto format(this Self&& self, const auto& value, FmtContext& ctx) {
    return std::visit(
        [&](const auto& s) { return self.handleSpec(value, ctx, s); },
        self.spec);
  }

  std::variant<Specs...> spec;
};

}  // namespace chesskit::internal

#endif  // CHESSKIT_INCLUDE_CHESSKIT_FORMATTER_INTERNAL_FORMATTER_HELPER_H_
