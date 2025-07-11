#ifndef CHESSKIT_INCLUDE_CHESSKIT_FORMATTER_POSITION_FORMATTER_H_
#define CHESSKIT_INCLUDE_CHESSKIT_FORMATTER_POSITION_FORMATTER_H_

// IWYU pragma: private, include "../position.h"

#include <format>

#include "../core/position.h"
#include "../core/position_impl.h"  // IWYU pragma: keep
#include "internal/formatter_helper.h"

/// @ingroup PositionHelpers
/// @brief formatting support for chesskit::Position
template <>
struct std::formatter<chesskit::Position>
    : chesskit::internal::SpecDispatcher<chesskit::internal::FenSpec,
                                         chesskit::internal::AsciiSpec,
                                         chesskit::internal::PieceListSpec,
                                         chesskit::internal::RepetitionSpec> {
  auto handleSpec(const chesskit::Position& position, auto& ctx,
                  chesskit::internal::FenSpec) const {
    return std::format_to(ctx.out(), "{:fen} {:c} {} {:?-} {} {}",
                          position.piecePlacement(), position.activeColor(),
                          position.castlingRights(),
                          position.enPassantTargetSquare(),
                          position.halfmoveClock(), position.fullmoveNumber());
  }

  auto handleSpec(const chesskit::Position& position, auto& ctx,
                  chesskit::internal::AsciiSpec) const {
    return std::format_to(
        ctx.out(),
        "{:ascii}\nActive color: {}\nCastling availability: {}\nEn passant "
        "target square: {:?-}\nHalfmove clock: {}\nFullmove number: {}",
        position.piecePlacement(), position.activeColor(),
        position.castlingRights(), position.enPassantTargetSquare(),
        position.halfmoveClock(), position.fullmoveNumber());
  }

  auto handleSpec(const chesskit::Position& position, auto& ctx,
                  chesskit::internal::PieceListSpec) const {
    return std::format_to(
        ctx.out(),
        "{:lists}\nActive color: {}\nCastling availability: {}\nEn passant "
        "target square: {:?-}\nHalfmove clock: {}\nFullmove number: {}",
        position.piecePlacement(), position.activeColor(),
        position.castlingRights(), position.enPassantTargetSquare(),
        position.halfmoveClock(), position.fullmoveNumber());
  }

  auto handleSpec(const chesskit::Position& position, auto& ctx,
                  chesskit::internal::RepetitionSpec) const {
    return std::format_to(ctx.out(), "{:fen} {:c} {} {:?-}",
                          position.piecePlacement(), position.activeColor(),
                          position.castlingRights(),
                          position.legalEnPassantTargetSquare());
  }
};

#endif  // CHESSKIT_INCLUDE_CHESSKIT_FORMATTER_POSITION_FORMATTER_H_
