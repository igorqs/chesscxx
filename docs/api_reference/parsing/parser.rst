Parser
======

.. doxygenclass:: chesskit::Parser

Standard specializations for library types
------------------------------------------

.. list-table::
   :widths: 100 
   :header-rows: 1

   * - Specialization
   * - :cpp:any:`chesskit::Parser\<CastlingRights, const char*, parse_as::Default> <template<> chesskit::Parser\<CastlingRights, const char*, parse_as::Default>>`
   * - :cpp:any:`chesskit::Parser\<CheckIndicator, const char*, parse_as::Default> <template<> chesskit::Parser\<CheckIndicator, const char*, parse_as::Default>>`
   * - :cpp:any:`chesskit::Parser\<Color, const char*, parse_as::Default> <template<> chesskit::Parser\<Color, const char*, parse_as::Default>>`
   * - :cpp:any:`chesskit::Parser\<File, const char*, parse_as::Default> <template<> chesskit::Parser\<File, const char*, parse_as::Default>>`
   * - :cpp:any:`chesskit::Parser\<Game, const char*, parse_as::Default> <template<> chesskit::Parser\<Game, const char*, parse_as::Default>>`
   * - :cpp:any:`chesskit::Parser\<Game, const char*, parse_as::Pgn> <template<> chesskit::Parser\<Game, const char*, parse_as::Pgn>>`
   * - :cpp:any:`chesskit::Parser\<Game, const char*, parse_as::Fen> <template<> chesskit::Parser\<Game, const char*, parse_as::Fen>>`
   * - :cpp:any:`chesskit::Parser\<GameResult, const char*, parse_as::Default> <template<> chesskit::Parser\<GameResult, const char*, parse_as::Default>>`
   * - :cpp:any:`chesskit::Parser\<PartialSquare, const char*, parse_as::Default> <template<> chesskit::Parser\<PartialSquare, const char*, parse_as::Default>>`
   * - :cpp:any:`chesskit::Parser\<Piece, const char*, parse_as::Default> <template<> chesskit::Parser\<Piece, const char*, parse_as::Default>>`
   * - :cpp:any:`chesskit::Parser\<PiecePlacement, const char*, parse_as::Default> <template<> chesskit::Parser\<PiecePlacement, const char*, parse_as::Default>>`
   * - :cpp:any:`chesskit::Parser\<PieceType, const char*, parse_as::Uppercase> <template<> chesskit::Parser\<PieceType, const char*, parse_as::Uppercase>>`
   * - :cpp:any:`chesskit::Parser\<PieceType, const char*, parse_as::Lowercase> <template<> chesskit::Parser\<PieceType, const char*, parse_as::Lowercase>>`
   * - :cpp:any:`chesskit::Parser\<PromotablePieceType, const char*, parse_as::Uppercase> <template<> chesskit::Parser\<PromotablePieceType, const char*, parse_as::Uppercase>>`
   * - :cpp:any:`chesskit::Parser\<PromotablePieceType, const char*, parse_as::Lowercase> <template<> chesskit::Parser\<PromotablePieceType, const char*, parse_as::Lowercase>>`
   * - :cpp:any:`chesskit::Parser\<Position, const char*, parse_as::Default> <template<> chesskit::Parser\<Position, const char*, parse_as::Default>>`
   * - :cpp:any:`chesskit::Parser\<Rank, const char*, parse_as::Default> <template<> chesskit::Parser\<Rank, const char*, parse_as::Default>>`
   * - :cpp:any:`chesskit::Parser\<SanNormalMove, const char*, parse_as::Default> <template<> chesskit::Parser\<SanNormalMove, const char*, parse_as::Default>>`
   * - :cpp:any:`chesskit::Parser\<SanCastlingMove, const char*, parse_as::Default> <template<> chesskit::Parser\<SanCastlingMove, const char*, parse_as::Default>>`
   * - :cpp:any:`chesskit::Parser\<SanMove, const char*, parse_as::Default> <template<> chesskit::Parser\<SanMove, const char*, parse_as::Default>>`
   * - :cpp:any:`chesskit::Parser\<Square, const char*, parse_as::Default> <template<> chesskit::Parser\<Square, const char*, parse_as::Default>>`
   * - :cpp:any:`chesskit::Parser\<UciMove, const char*, parse_as::Default> <template<> chesskit::Parser\<UciMove, const char*, parse_as::Default>>`

Examples
--------

.. literalinclude:: ../../../examples/parser_usage.cpp
   :language: cpp

Output:

.. literalinclude:: ../../../build/examples/parser_usage.out
   :language: none
