Parser
======

.. doxygenclass:: chesscxx::Parser

Standard specializations for library types
------------------------------------------

.. list-table::
   :widths: 100 
   :header-rows: 1

   * - Specialization
   * - :cpp:any:`chesscxx::Parser\<CastlingRights, const char*, parse_as::Default> <template<> chesscxx::Parser\<CastlingRights, const char*, parse_as::Default>>`
   * - :cpp:any:`chesscxx::Parser\<CheckIndicator, const char*, parse_as::Default> <template<> chesscxx::Parser\<CheckIndicator, const char*, parse_as::Default>>`
   * - :cpp:any:`chesscxx::Parser\<Color, const char*, parse_as::Default> <template<> chesscxx::Parser\<Color, const char*, parse_as::Default>>`
   * - :cpp:any:`chesscxx::Parser\<File, const char*, parse_as::Default> <template<> chesscxx::Parser\<File, const char*, parse_as::Default>>`
   * - :cpp:any:`chesscxx::Parser\<Game, const char*, parse_as::Default> <template<> chesscxx::Parser\<Game, const char*, parse_as::Default>>`
   * - :cpp:any:`chesscxx::Parser\<Game, const char*, parse_as::Pgn> <template<> chesscxx::Parser\<Game, const char*, parse_as::Pgn>>`
   * - :cpp:any:`chesscxx::Parser\<Game, const char*, parse_as::Fen> <template<> chesscxx::Parser\<Game, const char*, parse_as::Fen>>`
   * - :cpp:any:`chesscxx::Parser\<GameResult, const char*, parse_as::Default> <template<> chesscxx::Parser\<GameResult, const char*, parse_as::Default>>`
   * - :cpp:any:`chesscxx::Parser\<PartialSquare, const char*, parse_as::Default> <template<> chesscxx::Parser\<PartialSquare, const char*, parse_as::Default>>`
   * - :cpp:any:`chesscxx::Parser\<Piece, const char*, parse_as::Default> <template<> chesscxx::Parser\<Piece, const char*, parse_as::Default>>`
   * - :cpp:any:`chesscxx::Parser\<PiecePlacement, const char*, parse_as::Default> <template<> chesscxx::Parser\<PiecePlacement, const char*, parse_as::Default>>`
   * - :cpp:any:`chesscxx::Parser\<PieceType, const char*, parse_as::Uppercase> <template<> chesscxx::Parser\<PieceType, const char*, parse_as::Uppercase>>`
   * - :cpp:any:`chesscxx::Parser\<PieceType, const char*, parse_as::Lowercase> <template<> chesscxx::Parser\<PieceType, const char*, parse_as::Lowercase>>`
   * - :cpp:any:`chesscxx::Parser\<PromotablePieceType, const char*, parse_as::Uppercase> <template<> chesscxx::Parser\<PromotablePieceType, const char*, parse_as::Uppercase>>`
   * - :cpp:any:`chesscxx::Parser\<PromotablePieceType, const char*, parse_as::Lowercase> <template<> chesscxx::Parser\<PromotablePieceType, const char*, parse_as::Lowercase>>`
   * - :cpp:any:`chesscxx::Parser\<Position, const char*, parse_as::Default> <template<> chesscxx::Parser\<Position, const char*, parse_as::Default>>`
   * - :cpp:any:`chesscxx::Parser\<Rank, const char*, parse_as::Default> <template<> chesscxx::Parser\<Rank, const char*, parse_as::Default>>`
   * - :cpp:any:`chesscxx::Parser\<SanNormalMove, const char*, parse_as::Default> <template<> chesscxx::Parser\<SanNormalMove, const char*, parse_as::Default>>`
   * - :cpp:any:`chesscxx::Parser\<SanCastlingMove, const char*, parse_as::Default> <template<> chesscxx::Parser\<SanCastlingMove, const char*, parse_as::Default>>`
   * - :cpp:any:`chesscxx::Parser\<SanMove, const char*, parse_as::Default> <template<> chesscxx::Parser\<SanMove, const char*, parse_as::Default>>`
   * - :cpp:any:`chesscxx::Parser\<Square, const char*, parse_as::Default> <template<> chesscxx::Parser\<Square, const char*, parse_as::Default>>`
   * - :cpp:any:`chesscxx::Parser\<UciMove, const char*, parse_as::Default> <template<> chesscxx::Parser\<UciMove, const char*, parse_as::Default>>`

Examples
--------

.. literalinclude:: /../examples/parser_usage.cpp
   :language: cpp

Output:

.. literalinclude:: /examples_outputs/parser_usage.out
   :language: none
