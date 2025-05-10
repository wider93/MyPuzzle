#include "puzzlecore_pch.h"
#include "Random.h"

XorshiftRng::XorshiftRng(uint64_t seed) :state{ seed ? seed : 0x714ab9ba134ce } {
}

uint64_t XorshiftRng::next()
{
    state ^= state << 13;
    state ^= state >> 7;
    state ^= state << 17;
    return state;
}