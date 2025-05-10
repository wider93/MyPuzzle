#pragma once
#include "pch.h"
#include "Sudoku.h"

namespace Sudoku
{
    template <typename T>
    concept Req_HasEstimateCost = requires(const T & t)
    {
        { t.estimated_cost_const_impl() } -> std::convertible_to<int64_t>;
    };

    template <typename T>
    concept Req_OnSet = requires(const T s, SolverState & solver, value_t row, value_t col, value_t value)
    {
        { s.set_impl(solver, row, col, value) } -> std::same_as<bool>;
    };

    template <typename T>
    concept Req_OnElimination = requires(const T s, SolverState & solver, value_t row, value_t col, mask_t mask)
    {
        { s.on_eliminate_impl(solver, row, col, mask) } -> std::same_as<bool>;
    };

    template <typename Derived>
    class StrategyBase
    {
    public:

        bool on_set(SolverState& solver, value_t row, value_t col, value_t value) const
        {
            if constexpr (Req_OnSet<Derived>)
                return static_cast<const Derived*>(this)->on_eliminate_impl(solver, row, col, value);
            else
                return false;
        }

        bool on_eliminate(SolverState& solver, value_t row, value_t col, mask_t mask) const
        {
            if constexpr (Req_OnElimination<Derived>)
                return static_cast<const Derived*>(this)->on_eliminate_impl(solver, row, col, mask);
            else
                return false;
        }

        int64_t estimated_cost_const() const
        {
            if constexpr (Req_HasEstimateCost<Derived>)
                return static_cast<const Derived*>(this)->estimated_cost_const_impl();
            else
                return 1;
        }

    private:
        Req_OnElimination auto& impl()
        {
            return static_cast<Derived&>(*this);
        }
    };

    class PropagateUsedCandidate : public StrategyBase<PropagateUsedCandidate>
    {
        bool on_set_impl(SolverState& solver, value_t row, value_t col, value_t value) const
        {
            mask_t mask = 1u << value;

            for (int32_t r = 0; r < n; ++r)
            {
                if (r == row) continue;
                solver.push_elimination(r, col, mask);
            }

            for (int32_t c = 0; c < n; ++c)
            {
                if (c == col) continue;
                solver.push_elimination(row, c, mask);
            }

            int32_t r0 = row - row % 3, c0 = col - col % 3;

            for (int32_t r = r0; r < r0 + unit; ++r)
            {
                for (int32_t c = c0; c < c0 + unit; ++c)
                {
                    if (r == row && c == col) continue;
                    solver.push_elimination(r, c, mask);
                }
            }
        }

        bool on_eliminate_impl(SolverState& solver, value_t row, value_t col) const
        {
            uint32_t mask = solver.cells[row][col].availableMask;

            if ((mask & -mask) == mask)
            {
                value_t value = std::countr_zero(solver.cells[row][col].availableMask);
                solver.qSet.emplace_front(row, col, value);
                return true;
            }
            return false;
        }

        int64_t estimated_cost_const_impl() const
        {
            return 10;
        }
    };

    class DetectNakedSingle : public StrategyBase<DetectNakedSingle>
    {
    protected:
        bool on_set_impl(SolverState& solver, value_t row, value_t col, value_t value) const
        {
            return false;
        }

        bool on_eliminate_impl(SolverState& solver, value_t row, value_t col) const
        {
            uint32_t mask = solver.cells[row][col].availableMask;

            if ((mask & -mask) == mask)
            {
                value_t value = std::countr_zero(solver.cells[row][col].availableMask);
                solver.qSet.emplace_front(row, col, value);
                return true;
            }
            return false;
        }
    public:
        int64_t estimated_cost_const_impl() const
        {
            return 1;
        }
    };
}