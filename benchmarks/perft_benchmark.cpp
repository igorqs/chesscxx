#include <benchmark/benchmark.h>
#include <chesscxx/game.h>
#include <chesscxx/movegen.h>
#include <chesscxx/parse.h>

#include <cstdint>
#include <utility>

namespace {
auto Perft(chesscxx::Game& game, int64_t depth) -> int64_t {
  if (depth == 0) return 1ULL;

  int64_t nodes = 0;

  for (auto uci_move : chesscxx::legalUciMoves(game)) {
    std::ignore = game.move(uci_move);
    nodes += Perft(game, depth - 1);
    game.undoMove();
  }

  return nodes;
}

template <class... Args>
void BM_Perft(benchmark::State& state, Args&&... args) {
  auto args_tuple = std::make_tuple(std::forward<Args>(args)...);
  auto [fen, depth] = args_tuple;

  auto parsed_game =
      chesscxx::parse<chesscxx::Game>(fen, chesscxx::parse_as::Fen{});

  if (!parsed_game) return;

  for ([[maybe_unused]] auto ignore : state) {
    Perft(parsed_game.value(), depth);
  }
}
}  // namespace

BENCHMARK_CAPTURE(BM_Perft, position_1,
                  "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", 4)
    ->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(
    BM_Perft, position_2,
    "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1", 3)
    ->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BM_Perft, position_3,
                  "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1", 4)
    ->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(
    BM_Perft, position_4,
    "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1", 4)
    ->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(BM_Perft, position_5,
                  "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8",
                  3)
    ->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(
    BM_Perft, position_6,
    "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10",
    3)
    ->Unit(benchmark::kMillisecond);

BENCHMARK_MAIN();
