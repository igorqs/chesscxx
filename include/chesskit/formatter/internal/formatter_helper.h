#ifndef CHESSKIT_INCLUDE_CHESSKIT_FORMATTER_INTERNAL_FORMATTER_HELPER_H_
#define CHESSKIT_INCLUDE_CHESSKIT_FORMATTER_INTERNAL_FORMATTER_HELPER_H_

#include <iterator>
#include <string_view>
#include <variant>

#include "../base_formatter.h"

namespace chesskit::internal {

struct RepetitionSpec {
  static constexpr std::string_view kToken = "rep";
};
struct CompactUpperSpec {
  static constexpr std::string_view kToken = "u";
};
struct CompactLowerSpec {
  static constexpr std::string_view kToken = "l";
};
struct CompactSpec {
  static constexpr std::string_view kToken = "c";
};
struct VerboseSpec {
  static constexpr std::string_view kToken = "v";
};
struct PgnSpec {
  static constexpr std::string_view kToken = "pgn";
};
struct FenSpec {
  static constexpr std::string_view kToken = "fen";
};
struct AsciiSpec {
  static constexpr std::string_view kToken = "ascii";
};
struct PieceListSpec {
  static constexpr std::string_view kToken = "lists";
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
    auto ptr = ctx.begin();
    const auto end = ctx.end();

    std::string_view str(ctx);

    ([&] {
      if (str.size() < Specs::kToken.size()) return false;

      auto next = std::next(ptr, Specs::kToken.size());
      if (str.starts_with(Specs::kToken) && ((next == end) || *next == '}')) {
        spec = Specs{};
        ptr = next;
        return true;
      }
      return false;
    }() ||
     ...);

    return ptr;
  }

  template <typename Self, typename FmtContext>
  auto format(this const Self& self, const auto& value, FmtContext& ctx) {
    return std::visit(
        [&](const auto& spec) { return self.handleSpec(value, ctx, spec); },
        self.spec);
  }

  std::variant<Specs...> spec;
};

}  // namespace chesskit::internal

#endif  // CHESSKIT_INCLUDE_CHESSKIT_FORMATTER_INTERNAL_FORMATTER_HELPER_H_
