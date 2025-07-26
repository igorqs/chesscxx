Formatting
==========

The C++ Standard Library introduces ``std::formatter`` to define custom formatting rules for specific types. In ``chesskit``, most types provide their own ``std::formatter`` specializations. This allows you to use these types directly with standard formatting functions such as ``std::format``, ``std::print``, and related utilities.

std::formatter Specializations
------------------------------

.. list-table::
   :widths: 50 50
   :header-rows: 1

   * - Specialization
     - Supported format specifiers
   * - :cpp:any:`std::formatter\<chesskit::CastlingRights> <template<> std::formatter\<chesskit::CastlingRights>>`
     - ``{}(default)``
   * - :cpp:any:`std::formatter\<chesskit::CastlingSide> <template<> std::formatter\<chesskit::CastlingSide>>`
     - ``{}(default)``
   * - :cpp:any:`std::formatter\<chesskit::CheckIndicator> <template<> std::formatter\<chesskit::CheckIndicator>>`
     - ``{}(default)``, ``{:v}``, ``{:c}``
   * - :cpp:any:`std::formatter\<chesskit::Color> <template<> std::formatter\<chesskit::Color>>`
     - ``{}(default)``, ``{:v}``, ``{:c}``
   * - :cpp:any:`std::formatter\<chesskit::DrawReason> <template<> std::formatter\<chesskit::DrawReason>>`
     - ``{}(default)``
   * - :cpp:any:`std::formatter\<chesskit::File> <template<> std::formatter\<chesskit::File>>`
     - ``{}(default)``
   * - :cpp:any:`std::formatter\<chesskit::Game> <template<> std::formatter\<chesskit::Game>>`
     - ``{}(default)``, ``{:pgn}``, ``{:fen}``, ``{:ascii}``, ``{:lists}``, ``{:rep}``
   * - :cpp:any:`std::formatter\<chesskit::GameResult> <template<> std::formatter\<chesskit::GameResult>>`
     - ``{}(default)``, ``{:v}``, ``{:c}``
   * - :cpp:any:`std::formatter\<chesskit::MoveError> <template<> std::formatter\<chesskit::MoveError>>`
     - ``{}(default)``
   * - :cpp:any:`std::formatter\<chesskit::ParseError> <template<> std::formatter\<chesskit::ParseError>>`
     - ``{}(default)``
   * - :cpp:any:`std::formatter\<chesskit::PartialSquare> <template<> std::formatter\<chesskit::PartialSquare>>`
     - ``{}(default)``
   * - :cpp:any:`std::formatter\<chesskit::Piece> <template<> std::formatter\<chesskit::Piece>>`
     - ``{}(default)``, ``{:v}``, ``{:c}``
   * - :cpp:any:`std::formatter\<chesskit::PiecePlacementError> <template<> std::formatter\<chesskit::PiecePlacementError>>`
     - ``{}(default)``
   * - :cpp:any:`std::formatter\<chesskit::PiecePlacement> <template<> std::formatter\<chesskit::PiecePlacement>>`
     - ``{}(default)``, ``{:fen}``, ``{:ascii}``, ``{:lists}``
   * - :cpp:any:`std::formatter\<chesskit::PieceType> <template<> std::formatter\<chesskit::PieceType>>`
     - ``{}(default)``, ``{:v}``, ``{:u}``, ``{:l}``
   * - :cpp:any:`std::formatter\<chesskit::PromotablePieceType> <template<> std::formatter\<chesskit::PromotablePieceType>>`
     - ``{}(default)``, ``{:v}``, ``{:u}``, ``{:l}``
   * - :cpp:any:`std::formatter\<chesskit::PositionError> <template<> std::formatter\<chesskit::PositionError>>`
     - ``{}(default)``
   * - :cpp:any:`std::formatter\<chesskit::Position> <template<> std::formatter\<chesskit::Position>>`
     - ``{}(default)``, ``{:fen}``, ``{:ascii}``, ``{:lists}``, ``{:rep}``
   * - :cpp:any:`std::formatter\<chesskit::Rank> <template<> std::formatter\<chesskit::Rank>>`
     - ``{}(default)``
   * - :cpp:any:`std::formatter\<chesskit::SanCastlingMove> <template<> std::formatter\<chesskit::SanCastlingMove>>`
     - ``{}(default)``
   * - :cpp:any:`std::formatter\<chesskit::SanNormalMove> <template<> std::formatter\<chesskit::SanNormalMove>>`
     - ``{}(default)``
   * - :cpp:any:`std::formatter\<chesskit::SanMove> <template<> std::formatter\<chesskit::SanMove>>`
     - ``{}(default)``
   * - :cpp:any:`std::formatter\<chesskit::Square> <template<> std::formatter\<chesskit::Square>>`
     - ``{}(default)``
   * - :cpp:any:`std::formatter\<chesskit::UciMove> <template<> std::formatter\<chesskit::UciMove>>`
     - ``{}(default)``
   * - :cpp:any:`std::formatter\<std::optional\<T>> <template<typename T> std::formatter\<std::optional\<T>>>`
     - ``{:prefix[underlying_specs]suffix?default_value}``

std::formatter<std::optional<T>>
--------------------------------

.. doxygengroup:: OptionalFormatter
   :content-only:

Examples
~~~~~~~~

.. literalinclude:: /../examples/optional_formatter_usage.cpp
   :language: cpp

Output:

.. literalinclude:: /examples_outputs/optional_formatter_usage.out
   :language: none
