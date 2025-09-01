# A Modern C++ Chess Library

``chesscxx`` is a C++23 library for chess programming.  
It provides the essential tools for working with chess: simulating games, generating legal moves, parsing, and formatting.

## Features

- **Game simulation** — Create and manage chess games, apply validated moves, and query game state.  
- **Parsing** — Read moves (SAN, UCI), positions (FEN), and games (PGN).  
- **Formatting** — Convert moves, positions, and games to multiple string formats.  
- **Move generation** — Generate all legal moves for any position.  
- **Documentation** — Comprehensive API reference with examples. [See the docs →](https://igorqs.github.io/chesscxx/) 
- **Quality assurance** — Tested and validated to ensure correctness and reliability. 

## Benchmarks

### Perft

The following results were obtained by running `perft` benchmarks on (4 X 3241.88 MHz CPU s), measured with [Google Benchmark](https://github.com/google/benchmark).

| FEN                                                                      | Depth |Time (ms) |
|--------------------------------------------------------------------------|-------|----------|
| rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1                 | 4     | 198213   |
| r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1     | 3     | 135330   |
| 8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1                                | 4     | 22685    |
| r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1         | 4     | 460994   |
| rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8                | 3     | 62382    |
| r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10 | 3     | 91085    |

## Compiler Compatibility

| Compiler         | Minimum Version      |
|------------------|----------------------|
| GCC (libstdc++)  | 15.1                 |
