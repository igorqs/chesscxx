#include <benchmark/benchmark.h>
#include <chesscxx/game.h>
#include <chesscxx/movegen.h>

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

void BM_Perft(benchmark::State& state) {
  chesscxx::Game game;

  for ([[maybe_unused]] auto ignore : state) {
    Perft(game, state.range(0));
  }
}
}  // namespace

BENCHMARK(BM_Perft)->Arg(0)->Unit(benchmark::kMillisecond);
BENCHMARK(BM_Perft)->Arg(1)->Unit(benchmark::kMillisecond);
BENCHMARK(BM_Perft)->Arg(2)->Unit(benchmark::kMillisecond);
BENCHMARK(BM_Perft)->Arg(3)->Unit(benchmark::kMillisecond);
BENCHMARK(BM_Perft)->Arg(4)->Unit(benchmark::kMillisecond);

BENCHMARK_MAIN();
