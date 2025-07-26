#ifndef CHESSCXX_INCLUDE_CHESSCXX_FORMATTER_POSITION_FORMATTER_H_
#define CHESSCXX_INCLUDE_CHESSCXX_FORMATTER_POSITION_FORMATTER_H_

// IWYU pragma: private, include "../position.h"

#include <format>

#include "../core/position.h"
#include "../core/position_impl.h"  // IWYU pragma: keep
#include "internal/formatter_helper.h"

/// @ingroup PositionHelpers
/// @brief formatting support for chesscxx::Position
template <>
struct std::formatter<chesscxx::Position>
    : chesscxx::internal::SpecDispatcher<chesscxx::internal::FenSpec,
                                         chesscxx::internal::AsciiSpec,
                                         chesscxx::internal::PieceListSpec,
                                         chesscxx::internal::RepetitionSpec> {
  auto handleSpec(const chesscxx::Position& position, auto& ctx,
                  chesscxx::internal::FenSpec /*unused*/) const {
    return std::format_to(ctx.out(), "{:fen} {:c} {} {:?-} {} {}",
                          position.piecePlacement(), position.activeColor(),
                          position.castlingRights(),
                          position.enPassantTargetSquare(),
                          position.halfmoveClock(), position.fullmoveNumber());
  }

  auto handleSpec(const chesscxx::Position& position, auto& ctx,
                  chesscxx::internal::AsciiSpec /*unused*/) const {
    return std::format_to(
        ctx.out(),
        "{:ascii}\nActive color: {}\nCastling availability: {}\nEn passant "
        "target square: {:?-}\nHalfmove clock: {}\nFullmove number: {}",
        position.piecePlacement(), position.activeColor(),
        position.castlingRights(), position.enPassantTargetSquare(),
        position.halfmoveClock(), position.fullmoveNumber());
  }

  auto handleSpec(const chesscxx::Position& position, auto& ctx,
                  chesscxx::internal::PieceListSpec /*unused*/) const {
    return std::format_to(
        ctx.out(),
        "{:lists}\nActive color: {}\nCastling availability: {}\nEn passant "
        "target square: {:?-}\nHalfmove clock: {}\nFullmove number: {}",
        position.piecePlacement(), position.activeColor(),
        position.castlingRights(), position.enPassantTargetSquare(),
        position.halfmoveClock(), position.fullmoveNumber());
  }

  auto handleSpec(const chesscxx::Position& position, auto& ctx,
                  chesscxx::internal::RepetitionSpec /*unused*/) const {
    return std::format_to(ctx.out(), "{:fen} {:c} {} {:?-}",
                          position.piecePlacement(), position.activeColor(),
                          position.castlingRights(),
                          position.legalEnPassantTargetSquare());
  }
};

#endif  // CHESSCXX_INCLUDE_CHESSCXX_FORMATTER_POSITION_FORMATTER_H_
