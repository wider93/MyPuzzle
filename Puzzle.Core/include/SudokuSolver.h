#pragma once

#include "Sudoku.h"
#include "SudokuStrategy.h"

namespace Sudoku
{

    template <typename... Strategies>
    class Solver
    {
        std::tuple<Strategies...> strategies;
        SolverState state;

    public:
        Solver() : strategies{}, state{} {}
        //Solver(Strategies... &&s) : strategies(std::forward(s)...) {}

        bool solve(PlainBoard& board)
        {
            if (board.has_conflicts())
                return false;

            state.initiate_from_given_state(board);

            bool progress = true;

            while(!state.qSet.empty() || !state.qEliminate.empty())
            {
                bool progress = false;

                while (!state.qSet.empty())
                {
                    auto [r, c, t] = state.qSet.front();
                    state.qSet.pop_front();

                    if (state.set(r, c, t))
                    {
                        std::apply([&](auto&... strat)
                        {
                            ((progress = strat.on_set(state, r, c, t) ? true : progress), ...);
                        }, strategies);
                    }
                }
                
                if (!state.qEliminate.empty())
                {
                    auto [r, c] = state.qEliminate.front();
                    state.qEliminate.pop_front();

                    if (mask_t removed = state.eliminate(r, c))
                    {
                        std::apply([&](auto&... strat)
                        {
                            ((progress = strat.on_eliminate(state, r, c, removed) ? true : progress), ...);
                        }, strategies);
                    }
                }
            }

            state.set_board(board);

            return board.is_complete();
        }
    };

    template Solver<PropagateUsedCandidate, DetectNakedSingle>;
    template Solver<DetectNakedSingle>;
}