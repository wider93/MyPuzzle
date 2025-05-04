#include <benchmark/benchmark.h>
#include "Puzzle.Core.h"

int32_t f(int32_t n)
{
    int64_t j = 23;

    for (int32_t i = 0; i < n; ++i)
        j = (j + i) ^ i;

    return n;
}

static void BM_Check(benchmark::State& state)
{
    int fx = 0;
    int32_t n = (int32_t)state.range(0);
    for (const auto& _ : state)
    {
        n += sizeof(_);

        benchmark::DoNotOptimize(fx += f(n));
    }
    benchmark::DoNotOptimize(fx);
}
BENCHMARK(BM_Check)->Arg(9)->Arg(16000000);

static void BM_SolvePuzzle(benchmark::State& state)
{
    XorshiftRng rng;

    int32_t n = (int32_t)state.range(0);

    for (const auto& _ : state)
    {
        rng.next();
    }
    benchmark::DoNotOptimize(rng);
}

BENCHMARK(BM_SolvePuzzle)->Arg(9)->Arg(16000000)->Iterations(10000000);

BENCHMARK_MAIN();
