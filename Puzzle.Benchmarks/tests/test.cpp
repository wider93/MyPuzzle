#include <benchmark/benchmark.h>


static void BM_SolvePuzzle(benchmark::State& state)
{
    int32_t n = state.range(0);
    int64_t j = 0;
    for (int32_t i = 0; i < n; ++i)
        j += i;

}
BENCHMARK(BM_SolvePuzzle)->Arg(9)->Arg(16);