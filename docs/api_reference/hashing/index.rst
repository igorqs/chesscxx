Hashing
=======

The C++ Standard Library provides the ``std::hash`` template for defining custom hashing behavior. In ``chesscxx``, some types define ``std::hash`` specializations. This enables their use with standard unordered associative containers such as ``std::unordered_set``, ``std::unordered_multiset``, ``std::unordered_map``, and ``std::unordered_multimap``.

std::hash Specializations
-------------------------

.. list-table::
   :widths: 100
   :header-rows: 1

   * - Specialization
   * - :cpp:any:`std::hash\<chesscxx::CastlingRights> <template<> std::hash\<chesscxx::CastlingRights>>`
   * - :cpp:any:`std::hash\<chesscxx::Game> <template<> std::hash\<chesscxx::Game>>`
   * - :cpp:any:`std::hash\<chesscxx::Piece> <template<> std::hash\<chesscxx::Piece>>`
   * - :cpp:any:`std::hash\<chesscxx::PiecePlacement> <template<> std::hash\<chesscxx::PiecePlacement>>`
   * - :cpp:any:`std::hash\<chesscxx::Position> <template<> std::hash\<chesscxx::Position>>`
   * - :cpp:any:`std::hash\<chesscxx::Square> <template<> std::hash\<chesscxx::Square>>`
   * - :cpp:any:`std::hash\<chesscxx::UciMove> <template<> std::hash\<chesscxx::UciMove>>`

Hash and Equality predicates for repetition detection
-----------------------------------------------------

.. list-table::
   :widths: 100
   :header-rows: 1

   * - Type
   * - :cpp:any:`chesscxx::RepetitionEqual`
   * - :cpp:any:`chesscxx::RepetitionHash`
