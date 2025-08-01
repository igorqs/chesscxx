Position
========

.. doxygenclass:: chesscxx::Position
   :members: Position, fromParams, operator==, piecePlacement, activeColor, enPassantTargetSquare, legalEnPassantTargetSquare, halfmoveClock, fullmoveNumber, castlingRights, kMinHalfmoveClock, kMaxHalfmoveClock, kMinFullmoveNumber, kMaxFullmoveNumber

Nested Classes
--------------

.. doxygenstruct:: chesscxx::Position::Params
   :members:

Helper classes
--------------

.. doxygengroup:: PositionHelpers
   :content-only:

Examples
--------

General
~~~~~~~

.. includeexamplesource:: position_usage
   :language: cpp

Output:

.. includeexampleoutput:: position_usage
   :language: none

Creating from Params
~~~~~~~~~~~~~~~~~~~~

.. includeexamplesource:: position_error_usage
   :language: cpp

Output:

.. includeexampleoutput:: position_error_usage
   :language: none

Hashing
~~~~~~~

.. includeexamplesource:: position_hash_usage
   :language: cpp

Output:

.. includeexampleoutput:: position_hash_usage
   :language: none

Legal en passant target square
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. includeexamplesource:: position_legal_en_passant_usage
   :language: cpp

Output:

.. includeexampleoutput:: position_legal_en_passant_usage
   :language: none
