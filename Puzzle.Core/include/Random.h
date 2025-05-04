#pragma once


class XorshiftRng
{
    uint64_t state;
public:
    XorshiftRng(uint64_t seed = 0x714ab9ba134ce);

    uint64_t next();
};