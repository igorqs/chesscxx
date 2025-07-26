Position
========

.. doxygenclass:: chesskit::Position
   :members: Position, fromParams, operator==, piecePlacement, activeColor, enPassantTargetSquare, legalEnPassantTargetSquare, halfmoveClock, fullmoveNumber, castlingRights, kMinHalfmoveClock, kMaxHalfmoveClock, kMinFullmoveNumber, kMaxFullmoveNumber

Nested Classes
--------------

.. doxygenstruct:: chesskit::Position::Params
   :members:

Helper classes
--------------

.. doxygengroup:: PositionHelpers
   :content-only:

Examples
--------

General
~~~~~~~

.. literalinclude:: /../examples/position_usage.cpp
   :language: cpp

Output:

.. literalinclude:: /examples_outputs/position_usage.out
   :language: none

Creating from Params
~~~~~~~~~~~~~~~~~~~~

.. literalinclude:: /../examples/position_error_usage.cpp
   :language: cpp

Output:

.. literalinclude:: /examples_outputs/position_error_usage.out
   :language: none

Hashing
~~~~~~~

.. literalinclude:: /../examples/position_hash_usage.cpp
   :language: cpp

Output:

.. literalinclude:: /examples_outputs/position_hash_usage.out
   :language: none

Legal en passant target square
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. literalinclude:: /../examples/position_legal_en_passant_usage.cpp
   :language: cpp

Output:

.. literalinclude:: /examples_outputs/position_legal_en_passant_usage.out
   :language: none
