Formatting
==========

The C++ Standard Library introduces ``std::formatter`` to define custom formatting rules for specific types. In ``chesscxx``, most types provide their own ``std::formatter`` specializations. This allows you to use these types directly with standard formatting functions such as ``std::format``, ``std::print``, and related utilities.

std::formatter Specializations
------------------------------

.. list-table::
   :widths: 50 50
   :header-rows: 1

   * - Specialization
     - Supported format specifiers
   * - :cpp:any:`std::formatter\<chesscxx::CastlingRights> <template<> std::formatter\<chesscxx::CastlingRights>>`
     - ``{}(default)``
   * - :cpp:any:`std::formatter\<chesscxx::CastlingSide> <template<> std::formatter\<chesscxx::CastlingSide>>`
     - ``{}(default)``
   * - :cpp:any:`std::formatter\<chesscxx::CheckIndicator> <template<> std::formatter\<chesscxx::CheckIndicator>>`
     - ``{}(default)``, ``{:v}``, ``{:c}``
   * - :cpp:any:`std::formatter\<chesscxx::Color> <template<> std::formatter\<chesscxx::Color>>`
     - ``{}(default)``, ``{:v}``, ``{:c}``
   * - :cpp:any:`std::formatter\<chesscxx::DrawReason> <template<> std::formatter\<chesscxx::DrawReason>>`
     - ``{}(default)``
   * - :cpp:any:`std::formatter\<chesscxx::File> <template<> std::formatter\<chesscxx::File>>`
     - ``{}(default)``
   * - :cpp:any:`std::formatter\<chesscxx::Game> <template<> std::formatter\<chesscxx::Game>>`
     - ``{}(default)``, ``{:pgn}``, ``{:fen}``, ``{:ascii}``, ``{:lists}``, ``{:rep}``
   * - :cpp:any:`std::formatter\<chesscxx::GameResult> <template<> std::formatter\<chesscxx::GameResult>>`
     - ``{}(default)``, ``{:v}``, ``{:c}``
   * - :cpp:any:`std::formatter\<chesscxx::MoveError> <template<> std::formatter\<chesscxx::MoveError>>`
     - ``{}(default)``
   * - :cpp:any:`std::formatter\<chesscxx::ParseError> <template<> std::formatter\<chesscxx::ParseError>>`
     - ``{}(default)``
   * - :cpp:any:`std::formatter\<chesscxx::PartialSquare> <template<> std::formatter\<chesscxx::PartialSquare>>`
     - ``{}(default)``
   * - :cpp:any:`std::formatter\<chesscxx::Piece> <template<> std::formatter\<chesscxx::Piece>>`
     - ``{}(default)``, ``{:v}``, ``{:c}``
   * - :cpp:any:`std::formatter\<chesscxx::PiecePlacementError> <template<> std::formatter\<chesscxx::PiecePlacementError>>`
     - ``{}(default)``
   * - :cpp:any:`std::formatter\<chesscxx::PiecePlacement> <template<> std::formatter\<chesscxx::PiecePlacement>>`
     - ``{}(default)``, ``{:fen}``, ``{:ascii}``, ``{:lists}``
   * - :cpp:any:`std::formatter\<chesscxx::PieceType> <template<> std::formatter\<chesscxx::PieceType>>`
     - ``{}(default)``, ``{:v}``, ``{:u}``, ``{:l}``
   * - :cpp:any:`std::formatter\<chesscxx::PromotablePieceType> <template<> std::formatter\<chesscxx::PromotablePieceType>>`
     - ``{}(default)``, ``{:v}``, ``{:u}``, ``{:l}``
   * - :cpp:any:`std::formatter\<chesscxx::PositionError> <template<> std::formatter\<chesscxx::PositionError>>`
     - ``{}(default)``
   * - :cpp:any:`std::formatter\<chesscxx::Position> <template<> std::formatter\<chesscxx::Position>>`
     - ``{}(default)``, ``{:fen}``, ``{:ascii}``, ``{:lists}``, ``{:rep}``
   * - :cpp:any:`std::formatter\<chesscxx::Rank> <template<> std::formatter\<chesscxx::Rank>>`
     - ``{}(default)``
   * - :cpp:any:`std::formatter\<chesscxx::SanCastlingMove> <template<> std::formatter\<chesscxx::SanCastlingMove>>`
     - ``{}(default)``
   * - :cpp:any:`std::formatter\<chesscxx::SanNormalMove> <template<> std::formatter\<chesscxx::SanNormalMove>>`
     - ``{}(default)``
   * - :cpp:any:`std::formatter\<chesscxx::SanMove> <template<> std::formatter\<chesscxx::SanMove>>`
     - ``{}(default)``
   * - :cpp:any:`std::formatter\<chesscxx::Square> <template<> std::formatter\<chesscxx::Square>>`
     - ``{}(default)``
   * - :cpp:any:`std::formatter\<chesscxx::UciMove> <template<> std::formatter\<chesscxx::UciMove>>`
     - ``{}(default)``
   * - :cpp:any:`std::formatter\<std::optional\<T>> <template<typename T> std::formatter\<std::optional\<T>>>`
     - ``{:prefix[underlying_specs]suffix?default_value}``

std::formatter<std::optional<T>>
--------------------------------

.. doxygengroup:: OptionalFormatter
   :content-only:

Examples
~~~~~~~~

.. includeexamplesource:: optional_formatter_usage
   :language: cpp

Output:

.. includeexampleoutput:: optional_formatter_usage
   :language: none
