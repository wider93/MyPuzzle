#include "puzzlecore_pch.h"
#include "Sudoku.h"

using namespace Sudoku;

PlainBoard::Stage Sudoku::PlainBoard::check() const
{
    mask_t masks[n + n + n]{};

    mask_t* checkEachRow = masks;
    mask_t* checkEachCol = checkEachRow + n;
    mask_t* checkEachBox = checkEachCol + n;

    bool is_complete = true;

    for (int32_t i = 0; i < n; ++i)
    {
        for (int32_t j = 0; j < n; ++j)
        {
            value_t value = cells[i][j];

            if (value == 0)
            {
                is_complete = false;
                continue;
            }

            mask_t mask = 1u << value;

            if (checkEachCol[i] & mask)
                return conflicts;
            checkEachCol[i] |= mask;

            if (checkEachRow[j] & mask)
                return conflicts;
            checkEachRow[j] |= mask;

            if (checkEachBox[i - i % 3 + j % 3] & mask)
                return conflicts;
            checkEachBox[i - i % 3 + j % 3] |= mask;
        }
    }

    return is_complete ? complete : incomplete;
}

bool PlainBoard::has_conflicts() const
{
    return check() == conflicts;
}

bool PlainBoard::is_complete() const
{
    return check() == complete;
}

bool PlainBoard::save(const std::string& filename) const
{
    std::ofstream ofs(filename);
    if (!ofs) return false;

    for (int r = 0; r < n; ++r)
    {
        for (int c = 0; c < n; ++c)
            ofs << (char)('0' + cells[r][c]);

        ofs << '\n';
    }
    return true;
}

bool PlainBoard::load(const std::string& filename)
{
    std::ifstream ifs(filename);
    if (!ifs) return false;
    std::string line;
    for (int r = 0; r < n; ++r)
    {
        std::getline(ifs, line);
        for (int c = 0; c < n; ++c)
        {
            char ch = line[c];
            cells[r][c] = ch;
        }
    }
    return true;
}