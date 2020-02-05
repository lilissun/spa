//
//  if.cpp
//  calculus
//
//  Created by Li Li on 9/5/15.
//  Copyright (c) 2015 Lilissun. All rights reserved.
//

#include "if.h"

#include <sstream>

#include "common/iostream.h"
#include "common/debug.h"
#include "symbolic/term.h"
#include "symbolic/function.h"
#include "symbolic/tuple.h"
#include "symbolic/guard.h"
#include "symbolic/constraint.h"
#include "expression.h"
#include "state.h"

namespace cal {
    If::~If()
    {
        delete _left;
        delete _right;
        delete _then_process;
        delete _else_process;
    }
    
    void If::execute(Program *program, const State *state, std::vector<tim::Rule *> &rules) const
    {
        auto next_state = state->clone();
        auto left = _left->evaluate(program, next_state);
        auto right = _right->evaluate(program, next_state);
        ASSERT(_then_process != nullptr);

        auto left_sort = left->sort();
        auto right_sort = right->sort();
        if (_op == IfType::IF_EQUAL) {
            if ((left_sort == sym::TermSort::TERM_SORT_NUMBER || left_sort == sym::TERM_SORT_USER_DEFINED) && (right_sort == sym::TermSort::TERM_SORT_NUMBER || right_sort == sym::TermSort::TERM_SORT_USER_DEFINED)) {
                auto then_state = next_state->clone();
                then_state->unify(left, right);
                _then_process->execute(program, then_state, rules);
                if (_else_process != nullptr) {
                    next_state->add_guard(sym::make_guard_not_equal(left->clone(), right->clone()));
                    _else_process->execute(program, next_state, rules);
                }
                delete then_state;
            } else if ((left_sort == sym::TermSort::TERM_SORT_NUMBER || left_sort == sym::TERM_SORT_LINEAR_EXPRESSION) && (right_sort == sym::TermSort::TERM_SORT_NUMBER || right_sort == sym::TermSort::TERM_SORT_LINEAR_EXPRESSION)) {
                auto then_state = next_state->clone();
                then_state->add_constraint(sym::make_constraint_equal(left, right));
                _then_process->execute(program, then_state, rules);
                if (_else_process != nullptr) {
                    auto else_state = then_state->clone();
                    else_state->add_constraint(sym::make_constraint_not_equal(left, right));
                    _else_process->execute(program, else_state, rules);
                    delete else_state;
                }
                delete then_state;
            } else {
                std::stringstream ss;
                ss << "@if condition at " << _location << " is not equivalence";
                ss << " of linear expressions or user-defined functions";
                USER_ERROR(ss.str());
            }
        } else if (_op == IfType::IF_NOT_EQUAL) {
            if ((left_sort == sym::TermSort::TERM_SORT_NUMBER || left_sort == sym::TERM_SORT_USER_DEFINED) && (right_sort == sym::TermSort::TERM_SORT_NUMBER || right_sort == sym::TermSort::TERM_SORT_USER_DEFINED)) {
                auto then_state = next_state->clone();
                then_state->add_guard(sym::make_guard_not_equal(left, right));
                _then_process->execute(program, then_state, rules);
                if (_else_process != nullptr) {
                    next_state->unify(left->clone(), right->clone());
                    _else_process->execute(program, next_state, rules);
                }
                delete then_state;
            } else if ((left_sort == sym::TermSort::TERM_SORT_NUMBER || left_sort == sym::TERM_SORT_LINEAR_EXPRESSION) && (right_sort == sym::TermSort::TERM_SORT_NUMBER || right_sort == sym::TermSort::TERM_SORT_LINEAR_EXPRESSION)) {
                auto then_state = next_state->clone();
                then_state->add_constraint(sym::make_constraint_not_equal(left, right));
                _then_process->execute(program, then_state, rules);
                if (_else_process != nullptr) {
                    auto else_state = then_state->clone();
                    else_state->add_constraint(sym::make_constraint_equal(left, right));
                    _else_process->execute(program, else_state, rules);
                    delete else_state;
                }
                delete then_state;
            } else {
                std::stringstream ss;
                ss << "@if condition at " << _location << " is not inequivalence";
                ss << " of linear expressions or user-defined functions";
                USER_ERROR(ss.str());
            }
        } else {
            if ((left_sort == sym::TermSort::TERM_SORT_NUMBER || left_sort == sym::TERM_SORT_LINEAR_EXPRESSION) && (right_sort == sym::TermSort::TERM_SORT_NUMBER || right_sort == sym::TermSort::TERM_SORT_LINEAR_EXPRESSION)) {
                auto then_state = next_state->clone();
                sym::Constraint *constraint = nullptr;
                if (_op == IfType::IF_LESS) {
                    constraint = sym::make_constraint_less(left, right);
                } else if (_op == IfType::IF_LESS_EQUAL) {
                    constraint = sym::make_constraint_less_equal(left, right);
                } else if (_op == IfType::IF_GREATER) {
                    constraint = sym::make_constraint_greater(left, right);
                } else {
                    ASSERT(_op == IfType::IF_GREATER_EQUAL);
                    constraint = sym::make_constraint_greater_equal(left, right);
                }
                then_state->add_constraint(constraint);
                _then_process->execute(program, then_state, rules);
                if (_else_process != nullptr) {
                    auto else_state = next_state->clone();
                    auto reverse = constraint->reverse();
                    else_state->add_constraint(reverse);
                    _else_process->execute(program, else_state, rules);
                    delete else_state;
                }
                delete then_state;
            } else {
                std::stringstream ss;
                ss << "@if condition at " << _location << " is not inequivalence of linear expressions";
                USER_ERROR(ss.str());
            }
        }
        
        delete next_state;
    }
    
    void If::info(const size_t depth, std::ostream &os) const
    {
        os << com::tabs(process_tab_size, depth) << process_if << process_space << process_left_para;
        _left->info(os);
        os << process_space << IfTypeName[_op] << process_space;
        _right->info(os);
        os << process_right_para << process_space;
        if (_else_process == nullptr) {
            os << process_then << std::endl;
            _then_process->info(depth, os);
        } else {
            os << process_left_bracket << std::endl;
            _then_process->info(depth + 1, os);
            os << com::tabs(process_tab_size, depth) << process_right_bracket << process_space << process_else << process_space << process_left_bracket << std::endl;
            _else_process->info(depth + 1, os);
            os << com::tabs(process_tab_size, depth) << process_right_bracket << std::endl;
        }
    }
}