#include <gtest/gtest.h>

int grid[9][9];

TEST(SudokuGridTest, EmptyGridIsZero)
{
    for (int r = 0; r < 9; ++r)
    {
        for (int c = 0; c < 9; ++c)
        {
            EXPECT_EQ(grid[r][c], 0);
        }
    }
}