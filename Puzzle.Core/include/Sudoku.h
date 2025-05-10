#pragma once
#include "pch.h"

namespace Sudoku {
    
    using value_t = uint8_t;
    using mask_t = uint16_t;

    constexpr static value_t unit = 3;
    constexpr static value_t n = unit * unit;

    //#pragma pack(push, 1)

    struct Cell
    {
        value_t value;
        mask_t availableMask;
    };

    struct BoxRow
    {
        value_t values[unit];
        mask_t availableMask;
        mask_t alwaysMask;
    };

    struct BoxCol
    {
        value_t values[unit];
        mask_t availableMask;
        mask_t alwaysMask;
    };

    struct PlainBoard
    {
        enum class Stage
        {
            // a board with at least one blank cell and no explicit conflict.
            // a board destined to have conflict afterward belong here anyway.
            incomplete,

            // a totally filled board with no conflicts 
            complete,

            // a board with visible conflicts
            conflicts
        };
        using enum Stage;

        value_t cells[n][n];

        PlainBoard() : cells{} {}

        Stage check() const;
        bool has_conflicts() const;
        bool is_complete() const;
        bool save(const std::string& filename) const;
        bool load(const std::string& filename);
    };

    struct SolverState
    {
        Cell cells[n][n];

        std::deque<std::tuple<value_t, value_t, value_t>> qSet;
        std::deque<std::pair<value_t, value_t>> qEliminate;

        mask_t awaitingElimination[n][n];

        void set_board(PlainBoard& board) const
        {
            for (value_t r = 0; r < n; ++r)
                for (value_t c = 0; c < n; ++c)
                    board.cells[r][c] = cells[r][c].value;
        }

        void initiate_from_given_state(const PlainBoard& board)
        {
            qSet.clear();
            qEliminate.clear();
            memset(awaitingElimination, 0, n * n * sizeof(mask_t));

            for (value_t i = 0; i < n; ++i)
            {
                for (value_t j = 0; j < n; ++j)
                {
                    cells[i][j].value = 0;
                    cells[i][j].availableMask = (1u << (n + 1)) - 1;
                    value_t value = board.cells[i][j];

                    qSet.emplace_back(i, j, value);
                }
            }
        }

        bool set(value_t r, value_t c, value_t value)
        {
            if (cells[r][c].value == 0)
            {
                cells[r][c].value = value;
                cells[r][c].availableMask = 1u << value;
                return true;
            }
            return false;
        }

        bool is_awaiting_elimination(value_t r, value_t c, mask_t mask) const
        {
            return awaitingElimination[r][c] & mask;
        }

        mask_t push_elimination(value_t r, value_t c, mask_t mask)
        {
            if (!awaitingElimination[r][c]) qEliminate.emplace_back(r, c);

            awaitingElimination[r][c] |= mask;
        }

        mask_t eliminate(value_t r, value_t c)
        {
            mask_t remove = awaitingElimination[r][c];
            if (!remove) return 0;

            awaitingElimination[r][c] = 0;
            mask_t removed = cells[r][c].availableMask | remove;
            cells[r][c].availableMask &= ~remove;
            return removed;
        }
    };
}