# Contributing to chesscxx

Thank you for considering contributing to ``chesscxx``!
This document describes how to set up a development environment and use the available developer tools.

## Developer mode

Build system targets that are only useful for developers of this project are
hidden if the `chesscxx_DEVELOPER_MODE` option is disabled. Enabling this
option makes tests and other developer targets and options available. Not
enabling this option means that you are a consumer of this project and thus you
have no need for these targets and options.

### Prerequisites

To set up a development environment for `chesscxx`, you’ll need:

- **C++23** compatible compiler ([See the docs →](https://igorqs.github.io/chesscxx/#compiler-compatibility) for a list of supported compilers)
- **CMake** 3.28+ and a compatible generator (e.g., **Ninja**, **Make**, or **Visual Studio**)
- **clang-tidy** (for IWYU & static analysis checks)
- **clang-format** (for code formatting)
- **cppcheck** (for additional static analysis)
- **codespell** (for spell checking)
- **Doxygen** (for generating documentation)
  Extra packages: `sphinx`, `breathe`, `furo`, `sphinx-inline-tabs`, `myst-parser`
- **gcovr** (for coverage reports)
- **GTest**, **yaml-cpp**, **magic-enum** (for tests)

You can install most of these tools via your system package manager or using `pip` for Python-based tools.

### Docker

If you prefer, you can use Docker instead of installing all dependencies manually.  
A `Dockerfile` is provided at the project root that contains all required tools and libraries for building, testing, and running all targets of ``chesscxx``.

First, install Docker, then build the image:

```sh
docker build -t chesscxx .
```

To start an interactive container with the project mounted:

```sh
docker run --rm -it -v $(pwd):/chesscxx -w /chesscxx chesscxx
```

From inside the container, you can run all the usual commands (e.g., `cmake --preset=dev`, `cmake --build --preset=dev`, `ctest --preset=dev`).

Alternatively, instead of entering the container, you can run each command directly from your host system:

```sh
docker run --rm -v $(pwd):/chesscxx -w /chesscxx chesscxx <command>
# Examples:
docker run --rm -v $(pwd):/chesscxx -w /chesscxx chesscxx cmake --preset=dev -B docker-build
docker run --rm -v $(pwd):/chesscxx -w /chesscxx chesscxx cmake --build docker-build --preset=dev
docker run --rm -v $(pwd):/chesscxx -w /chesscxx chesscxx ctest --test-dir=docker-build --preset=dev
```

### Presets

This project makes use of [presets][1] to simplify the process of configuring
the project.

You have a few options to pass `chesscxx_DEVELOPER_MODE` to the configure
command, but this project prefers to use presets.

As a developer, you should create a `CMakeUserPresets.json` file at the root of
the project:

```json
{
  "version": 2,
  "cmakeMinimumRequired": {
    "major": 3,
    "minor": 28,
    "patch": 0
  },
  "configurePresets": [
    {
      "name": "dev",
      "binaryDir": "${sourceDir}/build/dev",
      "inherits": ["dev-mode", "ci-ubuntu"],
      "cacheVariables": {
        "CMAKE_BUILD_TYPE": "Debug",
        "CMAKE_EXPORT_COMPILE_COMMANDS": "ON"
      }
    }
  ],
  "buildPresets": [
    {
      "name": "dev",
      "configurePreset": "dev",
      "configuration": "Debug"
    }
  ],
  "testPresets": [
    {
      "name": "dev",
      "configurePreset": "dev",
      "configuration": "Debug",
      "output": {
        "outputOnFailure": true
      }
    }
  ]
}
```

`CMakeUserPresets.json` is also the perfect place in which you can put all
sorts of things that you would otherwise want to pass to the configure command
in the terminal.

### Configure, build and test

If you followed the above instructions, then you can configure, build and test
the project respectively with the following commands from the project root:

```sh
cmake --preset=dev
cmake --build --preset=dev
ctest --preset=dev
```

### Developer mode targets

These are targets you may invoke using the build command from above, with an
additional `-t <target>` flag:

#### `iwyu-check` and `iwyu-fix`

These targets run `clang-tidy` with the [misc-include-cleaner][2] check
on the header files to check IWYU errors and to fix them, respectively. 
Customization is available using the `IWYU_PATTERNS` and `IWYU_COMMAND`
cache variables.

#### `format-check` and `format-fix`

These targets run the `clang-format` tool on the codebase to check formatting errors and to
fix them, respectively. Customization is available using the `FORMAT_PATTERNS` and
`FORMAT_COMMAND` cache variables.

#### `spell-check` and `spell-fix`

These targets run the `codespell` tool on the codebase to check spelling errors and to fix
them, respectively. Customization is available using the `SPELL_COMMAND` cache
variable.

#### `run-examples` and `run_<name>`

Available if `BUILD_EXAMPLES` is enabled. Runs all the examples created by the `add_example` command,
or runs a single example.

#### `docs`

Builds the documentation using Doxygen + Breathe + Sphinx. The output will
go to `<binary-dir>/docs`. Customization is available using the `DOXYGEN_COMMAND`
and `SPHINX_COMMAND` cache variables.

Since the documentation uses the examples' outputs, you need to run the `run-examples`
target before building the documentation.

#### `coverage`

This target processes the output of the previously run tests and generates a coverage
report at `<build_dir>/coverage/index.html` using the `gcovr` tool. It requires the project
to be built using the coverage flags, which can be done by using the `ci-coverage` preset.
Customization is available using the `chesscxx_COVERAGE_COMMAND` cache variable.

[1]: https://cmake.org/cmake/help/latest/manual/cmake-presets.7.html
[2]: https://clang.llvm.org/extra/clang-tidy/checks/misc/include-cleaner.html
