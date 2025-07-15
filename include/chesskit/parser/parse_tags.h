#ifndef CHESSKIT_INCLUDE_CHESSKIT_PARSER_PARSE_TAGS_H_
#define CHESSKIT_INCLUDE_CHESSKIT_PARSER_PARSE_TAGS_H_

/// @brief Tag types for selecting parsing strategies.
namespace chesskit::parse_as {
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
}  // namespace chesskit::parse_as

#endif  // CHESSKIT_INCLUDE_CHESSKIT_PARSER_PARSE_TAGS_H_
