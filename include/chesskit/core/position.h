#ifndef CHESSKIT_INCLUDE_CHESSKIT_CORE_POSITION_H_
#define CHESSKIT_INCLUDE_CHESSKIT_CORE_POSITION_H_

// IWYU pragma: private, include "../position.h"

#include <cstdint>
#include <expected>
#include <limits>
#include <optional>

#include "../castling_rights.h"
#include "../color.h"
#include "../file.h"
#include "../piece.h"
#include "../piece_placement.h"
#include "../piece_type.h"
#include "../position_error.h"
#include "../rank.h"
#include "../square.h"
#include "internal/castling_rules.h"
#include "internal/piece_placement.h"
#include "internal/piece_placement_piece_at.h"
#include "internal/rank.h"
#include "internal/square.h"

namespace chesskit {

namespace internal {
// Forward declaration
class PositionModifier;
}  // namespace internal

/// @brief Represents a chess position.
class Position {
 public:
  /// @brief Minimum halfmove clock
  static constexpr uint32_t kMinHalfmoveClock = 0;

  /// @brief Maximum halfmove clock
  static constexpr uint32_t kMaxHalfmoveClock =
      std::numeric_limits<uint32_t>::max();

  /// @brief Minimum fullmove number
  static constexpr uint32_t kMinFullmoveNumber = 1;

  /// @brief Maximum fullmove number
  static constexpr uint32_t kMaxFullmoveNumber =
      std::numeric_limits<uint32_t>::max();

  /// @name Nested classes
  /// @{

  /// @brief Encapsulates the parameters required to construct a Position.
  struct Params {
    /// The layout of pieces on the board.
    PiecePlacement piecePlacement;
    /// The side to move next.
    Color activeColor = Color::kWhite;
    /// Castling rights available to both players.
    CastlingRights castlingRights;
    /// Target square for en passant, if applicable.
    std::optional<Square> enPassantTargetSquare;
    /// Number of halfmoves since the last capture or pawn move.
    uint32_t halfmoveClock = kMinHalfmoveClock;
    /// Number of full moves in the game.
    uint32_t fullmoveNumber = kMinFullmoveNumber;
  };

  /// @}

  /// @name Constructors
  /// @{

  /// @brief Default constructor. Constructs a Position representing the
  /// standard chess starting position.
  constexpr Position() = default;

  /// @}

  /// @name Static creation methods
  /// @{

  /// @brief Creates a Position from the given parameters, or returns an error
  /// if validation fails.
  static std::expected<Position, PositionError> fromParams(
      const Params& params) {
    if (!internal::isValidEnPassantTargetSquare(params.enPassantTargetSquare,
                                                params.activeColor)) {
      return std::unexpected(PositionError::kEnPassantTargetSquareInvalidRank);
    }

    Position position(params);

    if (auto error = position.validationError()) return std::unexpected(*error);

    return position;
  };

  /// @}

  /// @name Comparison operators
  /// @{

  /// @brief Equality comparison operator.
  constexpr bool operator==(const Position&) const = default;

  /// @}

  /// @name Element access
  /// @{

  /// @brief Returns the piece placement of the position.
  const PiecePlacement& piecePlacement() const { return piecePlacement_; }
  /// @brief Returns the active color (the side to move).
  const Color& activeColor() const { return activeColor_; }
  /// @brief Returns the castling rights.
  const CastlingRights& castlingRights() const { return castlingRights_; }
  /// @brief Returns the en passant target square, if any.
  std::optional<Square> enPassantTargetSquare() const {
    return enPassantFile_
               ? std::optional(Square(*enPassantFile_, enPassantRank()))
               : std::nullopt;
  }
  /// @brief Returns the en passant target square if it's legally capturable.
  std::optional<Square> legalEnPassantTargetSquare() const;
  /// @brief Returns the halfmove clock.
  const uint32_t& halfmoveClock() const { return halfmoveClock_; }
  /// @brief Returns the fullmove number.
  const uint32_t& fullmoveNumber() const { return fullmoveNumber_; }

  /// @}

 private:
  friend class internal::PositionModifier;

  explicit Position(const Params& params)
      : piecePlacement_{params.piecePlacement},
        activeColor_{params.activeColor},
        castlingRights_{params.castlingRights},
        enPassantFile_{internal::toOptionalFile(params.enPassantTargetSquare)},
        halfmoveClock_{params.halfmoveClock},
        fullmoveNumber_{params.fullmoveNumber} {}

  std::optional<PositionError> validationError() const {
    if (isFullmoveNumberOutOfRange()) {
      return PositionError::kFullmoveNumberOutOfRange;
    }
    if (isHalfmoveClockOutOfRange()) {
      return PositionError::kHalfmoveClockOutOfRange;
    }

    if (!internal::isValidCastlingRights(piecePlacement_, castlingRights_)) {
      return PositionError::kInvalidCastlingRightsForPiecePositions;
    }

    if (auto error = enPassantError()) return error;

    if (internal::isKingAttacked(piecePlacement_, !activeColor_)) {
      return PositionError::kSideNotToMoveIsUnderAttack;
    }

    return std::nullopt;
  }

  bool isFullmoveNumberOutOfRange() const {
    return fullmoveNumber_ < kMinFullmoveNumber ||
           fullmoveNumber_ > kMaxFullmoveNumber;
  }

  bool isHalfmoveClockOutOfRange() const {
    return halfmoveClock_ < kMinHalfmoveClock ||
           halfmoveClock_ > kMaxHalfmoveClock;
  }

  std::optional<PositionError> enPassantError() const {
    auto targetSquare = enPassantTargetSquare();
    if (!targetSquare) return std::nullopt;

    auto capturedPawnSquare = enPassantCapturedPawnSquare();
    if (!capturedPawnSquare) return std::nullopt;

    auto enemyPawn = Piece(PieceType::kPawn, !activeColor_);

    if (internal::hasPieceAt(piecePlacement_, *targetSquare)) {
      return PositionError::kEnPassantTargetSquareOccupied;
    }

    if (!internal::hasPieceAt(piecePlacement_, *capturedPawnSquare,
                              enemyPawn)) {
      return PositionError::kEnPassantNoCapturablePawn;
    }

    return std::nullopt;
  }

  std::optional<Square> enPassantCapturedPawnSquare() const {
    auto square = enPassantTargetSquare();
    if (!square) return std::nullopt;

    return internal::enPassantCapturedPawnSquare(*square, activeColor_);
  }

  Rank enPassantRank() const { return internal::enPassantRank(activeColor_); }

  void toggleActiveColor() { activeColor_ = !activeColor_; }
  void incrementMoveCounters() {
    incrementHalfmoveClock();
    incrementFullmoveNumber();
  }
  void incrementHalfmoveClock() { halfmoveClock_++; }
  void incrementFullmoveNumber() {
    if (activeColor_ == Color::kBlack) fullmoveNumber_++;
  }
  void resetHalfmoveClock() { halfmoveClock_ = 0; }
  void resetEnPassantFile() { enPassantFile_.reset(); }

  PiecePlacement piecePlacement_;
  Color activeColor_ = Color::kWhite;
  CastlingRights castlingRights_;
  std::optional<File> enPassantFile_;
  uint32_t halfmoveClock_ = kMinHalfmoveClock;
  uint32_t fullmoveNumber_ = kMinFullmoveNumber;
};

}  // namespace chesskit

#endif  // CHESSKIT_INCLUDE_CHESSKIT_CORE_POSITION_H_
