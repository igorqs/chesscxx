Hashing
=======

The C++ Standard Library provides the ``std::hash`` template for defining custom hashing behavior. In ``chesskit``, some types define ``std::hash`` specializations. This enables their use with standard unordered associative containers such as ``std::unordered_set``, ``std::unordered_multiset``, ``std::unordered_map``, and ``std::unordered_multimap``.

std::hash Specializations
-------------------------

.. list-table::
   :widths: 100
   :header-rows: 1

   * - Specialization
   * - :cpp:any:`std::hash\<chesskit::CastlingRights> <template<> std::hash\<chesskit::CastlingRights>>`
   * - :cpp:any:`std::hash\<chesskit::Game> <template<> std::hash\<chesskit::Game>>`
   * - :cpp:any:`std::hash\<chesskit::Piece> <template<> std::hash\<chesskit::Piece>>`
   * - :cpp:any:`std::hash\<chesskit::PiecePlacement> <template<> std::hash\<chesskit::PiecePlacement>>`
   * - :cpp:any:`std::hash\<chesskit::Position> <template<> std::hash\<chesskit::Position>>`
   * - :cpp:any:`std::hash\<chesskit::Square> <template<> std::hash\<chesskit::Square>>`
   * - :cpp:any:`std::hash\<chesskit::UciMove> <template<> std::hash\<chesskit::UciMove>>`

Hash and Equality predicates for repetition detection
-----------------------------------------------------

.. list-table::
   :widths: 100
   :header-rows: 1

   * - Type
   * - :cpp:any:`chesskit::RepetitionEqual`
   * - :cpp:any:`chesskit::RepetitionHash`
