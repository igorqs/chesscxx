#ifndef CHESSCXX_INCLUDE_CHESSCXX_FORMATTER_GAME_FORMATTER_H_
#define CHESSCXX_INCLUDE_CHESSCXX_FORMATTER_GAME_FORMATTER_H_

// IWYU pragma: private, include "../game.h"

#include <format>

#include "../color.h"
#include "../core/game.h"
#include "internal/formatter_helper.h"

/// @ingroup GameHelpers
/// @brief formatting support for chesscxx::Game
template <>
struct std::formatter<chesscxx::Game>
    : chesscxx::internal::SpecDispatcher<
          chesscxx::internal::PgnSpec, chesscxx::internal::FenSpec,
          chesscxx::internal::AsciiSpec, chesscxx::internal::PieceListSpec,
          chesscxx::internal::RepetitionSpec> {
  auto handleSpec(const chesscxx::Game& game, auto& ctx,
                  chesscxx::internal::PgnSpec /*unused*/) const {
    auto out = ctx.out();

    if (!game.startsFromDefaultPosition()) {
      out = std::format_to(
          out, "[Result \"{:[c]?*}\"]\n[FEN \"{:fen}\"]\n[SetUp \"1\"]\n\n",
          game.result(), game.initialPosition());
    } else {
      out = std::format_to(out, "[Result \"{:[c]?*}\"]\n\n", game.result());
    }

    const auto& moves = game.sanMoves();
    auto color = game.initialPosition().activeColor();
    auto move_number = game.initialPosition().fullmoveNumber();

    if (color == chesscxx::Color::kBlack && !moves.empty()) {
      out = std::format_to(out, "{}... ", move_number);
    }

    for (const auto& move : moves) {
      if (color == chesscxx::Color::kWhite) {
        out = std::format_to(out, "{}. ", move_number);
      }

      out = std::format_to(out, "{} ", move);

      if (color == chesscxx::Color::kBlack) move_number++;
      color = !color;
    }

    return std::format_to(out, "{:[c]?*}", game.result());
  }

  auto handleSpec(const auto& game, auto& ctx,
                  chesscxx::internal::FenSpec /*unused*/) const {
    return std::format_to(ctx.out(), "{:fen}", game.currentPosition());
  }

  auto handleSpec(const auto& game, auto& ctx,
                  chesscxx::internal::AsciiSpec /*unused*/) const {
    return std::format_to(ctx.out(), "{:ascii}", game.currentPosition());
  }

  auto handleSpec(const auto& game, auto& ctx,
                  chesscxx::internal::PieceListSpec /*unused*/) const {
    return std::format_to(ctx.out(), "{:lists}", game.currentPosition());
  }

  auto handleSpec(const auto& game, auto& ctx,
                  chesscxx::internal::RepetitionSpec /*unused*/) const {
    auto out = ctx.out();

    bool print_comma = false;
    out = std::format_to(out, "{{ ");
    for (const auto& [position, counter] : game.repetitionTracker()) {
      out = std::format_to(out, "{}\"{:rep}\": {}", print_comma ? ", " : "",
                           position, counter);
      print_comma = true;
    }
    out = std::format_to(out, " }}");

    return out;
  }
};

#endif  // CHESSCXX_INCLUDE_CHESSCXX_FORMATTER_GAME_FORMATTER_H_
