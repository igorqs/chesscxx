# ---- Dependencies ----

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/modules")
find_package(Doxygen REQUIRED)
find_package(Sphinx REQUIRED)

# ---- Declare Doxygen target ----

set(DOXYGEN_INPUT_DIR "${PROJECT_SOURCE_DIR}/include")
set(DOXYGEN_OUTPUT_DIR "${PROJECT_BINARY_DIR}/docs/doxygen")
set(DOXYGEN_INDEX_FILE "${DOXYGEN_OUTPUT_DIR}/xml/index.xml")
set(DOXYFILE_IN "${PROJECT_SOURCE_DIR}/docs/Doxyfile.in")
set(DOXYFILE_OUT "${PROJECT_BINARY_DIR}/docs/Doxyfile")

configure_file(${DOXYFILE_IN} ${DOXYFILE_OUT} @ONLY)

file(MAKE_DIRECTORY ${DOXYGEN_OUTPUT_DIR})

file(GLOB_RECURSE CHESSKIT_PUBLIC_HEADERS "${DOXYGEN_INPUT_DIR}/*.h")

# Only regenerate Doxygen when the Doxyfile or public headers change
add_custom_command(OUTPUT ${DOXYGEN_INDEX_FILE}
                   DEPENDS ${CHESSKIT_PUBLIC_HEADERS}
                   COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYFILE_OUT}
                   MAIN_DEPENDENCY ${DOXYFILE_OUT} ${DOXYFILE_IN}
                   COMMENT "Generating docs"
                   VERBATIM)

add_custom_target(doxygen DEPENDS ${DOXYGEN_INDEX_FILE})

# ---- Declare Sphinx target ----

set(SPHINX_SOURCE "${PROJECT_SOURCE_DIR}/docs")
set(SPHINX_BUILD "${PROJECT_BINARY_DIR}/docs/sphinx")
set(SPHINX_INDEX_FILE "${SPHINX_BUILD}/index.html")
set(WORKING_DIR "${PROJECT_BINARY_DIR}/docs")

file(GLOB_RECURSE SPHINX_RST_FILES "${SPHINX_SOURCE}/*.rst")

set(EXAMPLES_BUILD_DIR "${PROJECT_BINARY_DIR}/examples/")
set(DOCS_EXAMPLES_OUTPUTS_DIR "${PROJECT_SOURCE_DIR}/docs/examples_outputs/")

file(GLOB OUT_FILES "${EXAMPLES_BUILD_DIR}/*.out")
file(COPY ${OUT_FILES} DESTINATION ${DOCS_EXAMPLES_OUTPUTS_DIR})

# Only regenerate Sphinx when:
# - Doxygen has rerun
# - Our doc files have been updated
# - The Sphinx config has been updated
add_custom_command(OUTPUT ${SPHINX_INDEX_FILE}
                   COMMAND 
                     ${SPHINX_EXECUTABLE} -b html
                     -Dbreathe_projects.ChessKit="${DOXYGEN_OUTPUT_DIR}/xml"
                     ${SPHINX_SOURCE} ${SPHINX_BUILD}
                   WORKING_DIRECTORY ${WORKING_DIR}
                   DEPENDS
                   ${SPHINX_RST_FILES}
                   ${DOXYGEN_INDEX_FILE}
                   MAIN_DEPENDENCY ${SPHINX_SOURCE}/conf.py
                   COMMENT "Generating documentation with Sphinx")

add_custom_target(sphinx DEPENDS ${SPHINX_INDEX_FILE})
