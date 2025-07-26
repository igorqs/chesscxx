#ifndef CHESSCXX_INCLUDE_CHESSCXX_CORE_POSITION_H_
#define CHESSCXX_INCLUDE_CHESSCXX_CORE_POSITION_H_

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

namespace chesscxx {

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
    PiecePlacement piece_placement;
    /// The side to move next.
    Color active_color = Color::kWhite;
    /// Castling rights available to both players.
    CastlingRights castling_rights;
    /// Target square for en passant, if applicable.
    std::optional<Square> en_passant_target_square;
    /// Number of halfmoves since the last capture or pawn move.
    uint32_t halfmove_clock = kMinHalfmoveClock;
    /// Number of full moves in the game.
    uint32_t fullmove_number = kMinFullmoveNumber;
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
  static auto fromParams(const Params& params)
      -> std::expected<Position, PositionError> {
    if (!internal::isValidEnPassantTargetSquare(params.en_passant_target_square,
                                                params.active_color)) {
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
  constexpr auto operator==(const Position&) const -> bool;

  /// @}

  /// @name Element access
  /// @{

  /// @brief Returns the piece placement of the position.
  auto piecePlacement() const -> const PiecePlacement& {
    return piece_placement_;
  }
  /// @brief Returns the active color (the side to move).
  auto activeColor() const -> const Color& { return active_color_; }
  /// @brief Returns the castling rights.
  auto castlingRights() const -> const CastlingRights& {
    return castling_rights_;
  }
  /// @brief Returns the en passant target square, if any.
  auto enPassantTargetSquare() const -> std::optional<Square> {
    return en_passant_file_
               ? std::optional(Square(*en_passant_file_, enPassantRank()))
               : std::nullopt;
  }
  /// @brief Returns the en passant target square if it's legally capturable.
  auto legalEnPassantTargetSquare() const -> std::optional<Square>;
  /// @brief Returns the halfmove clock.
  auto halfmoveClock() const -> const uint32_t& { return halfmove_clock_; }
  /// @brief Returns the fullmove number.
  auto fullmoveNumber() const -> const uint32_t& { return fullmove_number_; }

  /// @}

 private:
  friend class internal::PositionModifier;

  explicit Position(const Params& params)
      : piece_placement_{params.piece_placement},
        active_color_{params.active_color},
        castling_rights_{params.castling_rights},
        en_passant_file_{
            internal::toOptionalFile(params.en_passant_target_square)},
        halfmove_clock_{params.halfmove_clock},
        fullmove_number_{params.fullmove_number} {}

  auto validationError() const -> std::optional<PositionError> {
    if (isFullmoveNumberOutOfRange()) {
      return PositionError::kFullmoveNumberOutOfRange;
    }
    if (isHalfmoveClockOutOfRange()) {
      return PositionError::kHalfmoveClockOutOfRange;
    }

    if (!internal::isValidCastlingRights(piece_placement_, castling_rights_)) {
      return PositionError::kInvalidCastlingRightsForPiecePositions;
    }

    if (auto error = enPassantError()) return error;

    if (internal::isKingAttacked(piece_placement_, !active_color_)) {
      return PositionError::kSideNotToMoveIsUnderAttack;
    }

    return std::nullopt;
  }

  auto isFullmoveNumberOutOfRange() const -> bool {
    return fullmove_number_ < kMinFullmoveNumber ||
           fullmove_number_ > kMaxFullmoveNumber;
  }

  auto isHalfmoveClockOutOfRange() const -> bool {
    return halfmove_clock_ < kMinHalfmoveClock ||
           halfmove_clock_ > kMaxHalfmoveClock;
  }

  auto enPassantError() const -> std::optional<PositionError> {
    auto target_square = enPassantTargetSquare();
    if (!target_square) return std::nullopt;

    auto captured_pawn_square = enPassantCapturedPawnSquare();
    if (!captured_pawn_square) return std::nullopt;

    auto enemy_pawn = Piece(PieceType::kPawn, !active_color_);

    if (internal::hasPieceAt(piece_placement_, *target_square)) {
      return PositionError::kEnPassantTargetSquareOccupied;
    }

    if (!internal::hasPieceAt(piece_placement_, *captured_pawn_square,
                              enemy_pawn)) {
      return PositionError::kEnPassantNoCapturablePawn;
    }

    return std::nullopt;
  }

  auto enPassantCapturedPawnSquare() const -> std::optional<Square> {
    auto square = enPassantTargetSquare();
    if (!square) return std::nullopt;

    return internal::enPassantCapturedPawnSquare(*square, active_color_);
  }

  auto enPassantRank() const -> Rank {
    return internal::enPassantRank(active_color_);
  }

  void toggleActiveColor() { active_color_ = !active_color_; }
  void incrementMoveCounters() {
    incrementHalfmoveClock();
    incrementFullmoveNumber();
  }
  void incrementHalfmoveClock() { halfmove_clock_++; }
  void incrementFullmoveNumber() {
    if (active_color_ == Color::kBlack) fullmove_number_++;
  }
  void resetHalfmoveClock() { halfmove_clock_ = 0; }
  void resetEnPassantFile() { en_passant_file_.reset(); }

  PiecePlacement piece_placement_;
  Color active_color_ = Color::kWhite;
  CastlingRights castling_rights_;
  std::optional<File> en_passant_file_;
  uint32_t halfmove_clock_ = kMinHalfmoveClock;
  uint32_t fullmove_number_ = kMinFullmoveNumber;
};

constexpr auto Position::operator==(const Position&) const -> bool = default;

}  // namespace chesscxx

#endif  // CHESSCXX_INCLUDE_CHESSCXX_CORE_POSITION_H_
