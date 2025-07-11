#ifndef CHESSKIT_INCLUDE_CHESSKIT_PARSER_PARSE_TAGS_H_
#define CHESSKIT_INCLUDE_CHESSKIT_PARSER_PARSE_TAGS_H_

namespace chesskit {

/// @brief Tag types for selecting parsing strategies.
namespace parse_as {
/// @brief %Default parsing strategy.
struct Default {};
/// @brief Forsyth–Edwards Notation (FEN) parsing.
struct Fen {};
/// @brief Portable %Game Notation (PGN) parsing.
struct Pgn {};
/// @brief %Uppercase parsing.
struct Uppercase {};
/// @brief %Lowercase parsing.
struct Lowercase {};
}  // namespace parse_as

}  // namespace chesskit

#endif  // CHESSKIT_INCLUDE_CHESSKIT_PARSER_PARSE_TAGS_H_
