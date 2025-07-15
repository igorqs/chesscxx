#ifndef CHESSKIT_INCLUDE_CHESSKIT_CORE_GAME_H_
#define CHESSKIT_INCLUDE_CHESSKIT_CORE_GAME_H_

// IWYU pragma: private, include "../game.h"

#include <cstdint>
#include <expected>
#include <optional>
#include <unordered_map>
#include <vector>

#include "../castling_rights.h"
#include "../color.h"
#include "../draw_reason.h"
#include "../game_result.h"
#include "../hash/position_hash.h"
#include "../move_error.h"
#include "../piece_placement.h"
#include "../position.h"
#include "../san_move.h"
#include "../square.h"
#include "../uci_move.h"
#include "internal/move_record.h"
#include "internal/position.h"
#include "internal/position_modifier.h"

namespace chesskit {

/// @brief Represents a chess game.
class Game {
 public:
  /// @brief Type alias for mapping from positions to their repetition count,
  /// used to detect threefold repetition.
  using RepetitionTracker =
      std::unordered_map<Position, uint32_t, RepetitionHash, RepetitionEqual>;

  /// @name Constructors
  /// @{

  /// @brief Default constructor. Constructs a new game starting from the
  /// default position.
  constexpr Game() { updateRepetitionTracker(); }

  /// @brief Constructs a new game with a specified initial position.
  constexpr explicit Game(const Position& initialPosition)
      : initialPosition_(initialPosition),
        currentPosition_(initialPosition),
        isDefaultStart_(initialPosition == Position{}) {
    updateRepetitionTracker();
  }

  /// @}

  /// @name Comparison operators
  /// @{

  /// @brief Equality comparison operator.
  constexpr auto operator==(const Game& other) const -> bool {
    return initialPosition_ == other.initialPosition_ &&
           uciMoveHistory_ == other.uciMoveHistory_;
  };

  /// @}

  /// @name Element access
  /// @{

  /// @brief Returns the initial game position.
  auto initialPosition() const -> const Position& { return initialPosition_; }
  /// @brief Indicates whether the game started from the default position.
  auto startsFromDefaultPosition() const -> bool { return isDefaultStart_; }

  /// @brief Returns the current game position.
  auto currentPosition() const -> const Position& { return currentPosition_; }

  /// @brief Returns the piece placement of the current position.
  auto piecePlacement() const -> const PiecePlacement& {
    return currentPosition_.piecePlacement();
  }
  /// @brief Returns the active color (the side to move).
  auto activeColor() const -> const Color& {
    return currentPosition_.activeColor();
  }
  /// @brief Returns the en passant target square, if any.
  auto enPassantTargetSquare() const -> std::optional<Square> {
    return currentPosition_.enPassantTargetSquare();
  }
  /// @brief Returns the en passant target square if it's legally capturable.
  auto legalEnPassantTargetSquare() const -> std::optional<Square> {
    return currentPosition_.legalEnPassantTargetSquare();
  }
  /// @brief Returns the number of halfmoves since the last capture or pawn
  /// move.
  auto halfmoveClock() const -> const uint32_t& {
    return currentPosition_.halfmoveClock();
  }
  /// @brief Returns the current fullmove number.
  auto fullmoveNumber() const -> const uint32_t& {
    return currentPosition_.fullmoveNumber();
  }
  /// @brief Returns the current castling rights.
  auto castlingRights() const -> const CastlingRights& {
    return currentPosition_.castlingRights();
  }

  /// @brief Returns the list of moves in UCI notation played since the initial
  /// position.
  auto uciMoves() const -> const std::vector<UciMove>& {
    return uciMoveHistory_;
  }

  /// @brief Returns the list of moves in SAN notation played since the initial
  /// position.
  auto sanMoves() const -> const std::vector<SanMove>& {
    return sanMoveHistory_;
  }

  /// @brief Returns the repetition tracker.
  auto repetitionTracker() const -> const RepetitionTracker& {
    return repetitionTracker_;
  }

  ///  @brief Returns the result of the game if it is over.
  auto result() const -> std::optional<GameResult> {
    if (isDraw()) return GameResult::kDraw;
    if (isCheckmate()) {
      if (activeColor() == Color::kWhite) return GameResult::kBlackWins;
      return GameResult::kWhiteWins;
    }

    return std::nullopt;
  }

  /// @brief Returns the draw reason if the game ended in a draw.
  auto drawReason() const -> std::optional<DrawReason> {
    if (isFiftyMoveRuleDraw()) return DrawReason::kFiftyMoveRule;
    if (isInsufficientMaterialDraw()) return DrawReason::kInsufficientMaterial;
    if (isThreefoldRepetition()) return DrawReason::kThreefoldRepetition;
    if (isStalemate()) return DrawReason::kStalemate;

    return std::nullopt;
  }

  /// @}

  /// @name Modifiers
  /// @{

  /// @brief Applies a move specified in UCI (Universal Chess Interface) format
  /// to the current game state, or returns an error if the move is illegal for
  /// the current position.
  auto move(const UciMove& move) -> std::expected<void, MoveError> {
    return executeMove(move);
  }
  /// @brief Applies a move specified in SAN (Standard Algebraic Notation)
  /// format to the current game state, or returns an error if the move is
  /// illegal for the current position.
  auto move(const SanMove& move) -> std::expected<void, MoveError> {
    return executeMove(move);
  }

  /// @brief Undoes the last move played, restoring the previous position.
  void undoMove() {
    if (historyIsEmpty()) return;

    removePositionOccurrence();

    const auto& move = lastMove();
    internal::PositionModifier::undoMove(currentPosition_, move);

    popBackHistory();
  }

  /// @brief Resets the game to the initial position
  void reset() {
    currentPosition_ = initialPosition_;

    clearHistory();

    clearRepetitionTracker();
    updateRepetitionTracker();
  }

  /// @}

 private:
  template <typename MoveInput>
  auto executeMove(const MoveInput& move) -> std::expected<void, MoveError> {
    auto result = internal::PositionModifier::move(currentPosition_, move);

    if (result) {
      addToHistory(*result);
      updateRepetitionTracker();
      return {};
    }

    return std::unexpected(result.error());
  }

  auto isGameOver() const -> bool { return isCheckmate() || isDraw(); }
  auto isCheckmate() const -> bool {
    return internal::isCheckmate(currentPosition_);
  }
  auto isDraw() const -> bool {
    return internal::isDraw(currentPosition_) || isThreefoldRepetition();
  }
  auto isStalemate() const -> bool {
    return internal::isStalemate(currentPosition_);
  }
  auto isFiftyMoveRuleDraw() const -> bool {
    return internal::isFiftyMoveRuleDraw(currentPosition_);
  }
  auto isInsufficientMaterialDraw() const -> bool {
    return internal::isInsufficientMaterialDraw(currentPosition_);
  }
  auto isThreefoldRepetition() const -> bool {
    return repetitionTracker_.at(currentPosition_) >= 3;
  }

  auto historyIsEmpty() -> bool { return moveHistory_.empty(); }
  void clearHistory() {
    moveHistory_.clear();
    uciMoveHistory_.clear();
    sanMoveHistory_.clear();
  }
  void addToHistory(const internal::MoveRecord& move) {
    moveHistory_.push_back(move);
    uciMoveHistory_.push_back(internal::convertTo<UciMove>(move));
    sanMoveHistory_.push_back(internal::convertTo<SanMove>(move));
  }
  void popBackHistory() {
    moveHistory_.pop_back();
    uciMoveHistory_.pop_back();
    sanMoveHistory_.pop_back();
  }
  auto lastMove() -> const internal::MoveRecord& {
    return moveHistory_.back();
  };

  void clearRepetitionTracker() { repetitionTracker_.clear(); }
  void removePositionOccurrence() {
    repetitionTracker_[currentPosition_]--;
    if (repetitionTracker_[currentPosition_] == 0) {
      repetitionTracker_.erase(currentPosition_);
    }
  }
  void updateRepetitionTracker() { repetitionTracker_[currentPosition_]++; }

  Position initialPosition_;
  bool isDefaultStart_ = true;
  Position currentPosition_;
  std::vector<internal::MoveRecord> moveHistory_;
  std::vector<UciMove> uciMoveHistory_;
  std::vector<SanMove> sanMoveHistory_;
  RepetitionTracker repetitionTracker_;
};

}  // namespace chesskit

#endif  // CHESSKIT_INCLUDE_CHESSKIT_CORE_GAME_H_
