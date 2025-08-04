Getting Started
===============

Installation
------------

``chesscxx`` is a **header-only** library, meaning no compilation or linking is required.  
To use it in your project, simply add the `include` directory to your compiler's include path.

For example:

.. code-block:: bash

   git clone https://github.com/igorqs/chesscxx.git
   g++ -I chesscxx/include your_program.cpp -o your_program -std=c++23


Building with CMake
~~~~~~~~~~~~~~~~~~~

If you prefer, you can build and install ``chesscxx`` using CMake.  
This allows you to install the headers system-wide (e.g., into `/usr/local/include/chesscxx-<version>/`).

Build
^^^^^

For single-configuration generators (like Unix Makefiles):

.. code-block:: bash

   cmake -S . -B build -D CMAKE_BUILD_TYPE=Release
   cmake --build build

For multi-configuration generators (like Visual Studio):

.. code-block:: bash

   cmake -S . -B build
   cmake --build build --config Release

Install
^^^^^^^

After building, install the headers:

For single-configuration generators:

.. code-block:: bash

   cmake --install build

For multi-configuration generators:

.. code-block:: bash

  cmake --install build --config Release

CMake package
"""""""""""""

Once installed, other CMake projects (targets) can depend on ``chesscxx`` by using the `find_package <https://cmake.org/cmake/help/latest/command/find_package.html>`_
command of CMake:

* Package name: `chesscxx`
* Target name: `chesscxx::chesscxx`

Example usage:

.. code-block:: cmake

  find_package(chesscxx REQUIRED)
  # Declare the imported target as a build requirement using PRIVATE, where
  # project_target is a target created in the consuming project
  target_link_libraries(
      project_target PRIVATE
      chesscxx::chesscxx
  )

Using with FetchContent
~~~~~~~~~~~~~~~~~~~~~~~

If you prefer not to install ``chesscxx`` system-wide, you can use CMake's 
`FetchContent <https://cmake.org/cmake/help/latest/module/FetchContent.html>`_ module 
to fetch the library directly from its repository at configure time:

.. code-block:: cmake

   include(FetchContent)
   FetchContent_Declare(
       chesscxx
       GIT_REPOSITORY https://github.com/igorqs/chesscxx.git
       GIT_TAG main   # <-- or a specific commit/tag for stability
   )
   FetchContent_MakeAvailable(chesscxx)

   target_link_libraries(
       project_target PRIVATE
       chesscxx::chesscxx
   )

Examples
--------

Full Game
~~~~~~~~~

.. includeexamplesource:: basic_full_game_usage
   :language: cpp

Output:

.. includeexampleoutput:: basic_full_game_usage
   :language: none

Using the Move Generator
~~~~~~~~~~~~~~~~~~~~~~~~

.. includeexamplesource:: movegen_promotion_usage
   :language: cpp

Output:

.. includeexampleoutput:: movegen_promotion_usage
   :language: none

Parsing PGN
~~~~~~~~~~~

.. includeexamplesource:: basic_pgn_usage
   :language: cpp

Output:

.. includeexampleoutput:: basic_pgn_usage
   :language: none

Parsing games from a file
~~~~~~~~~~~~~~~~~~~~~~~~~

.. includeexamplesource:: parse_from_usage
   :language: cpp

.. tab:: Output:

    .. includeexampleoutput:: parse_from_usage
       :language: none

.. tab:: Input file:

    .. includeexampledata:: data/games.pgn
       :language: none
