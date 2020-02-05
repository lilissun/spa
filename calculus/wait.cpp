//
//  wait.cpp
//  calculus
//
//  Created by Li Li on 9/5/15.
//  Copyright (c) 2015 Lilissun. All rights reserved.
//

#include "wait.h"

#include <sstream>

#include "common/iostream.h"
#include "common/debug.h"
#include "symbolic/term.h"
#include "symbolic/number.h"
#include "symbolic/function.h"
#include "symbolic/constant.h"
#include "symbolic/tuple.h"
#include "symbolic/pair.h"
#include "symbolic/constraint.h"
#include "expression.h"
#include "tuple.h"
#include "name.h"
#include "state.h"
#include "program.h"

namespace cal {
    Wait::~Wait()
    {
        delete _timestamp;
        delete _left;
        delete _right;
        delete _next_process;
    }
    
    void Wait::execute(Program *program, const State *state, std::vector<tim::Rule *> &rules) const
    {
        State *next_state = state->clone();
        
        if (_clock == default_global_clock) {
            auto new_global_timestamp = (sym::Number *) next_state->declare_timestamp(_timestamp->name(), _location)->clone();
            auto latest_global_timestamp = (sym::Number *) next_state->get_latest_timestamp(default_global_clock)->clone();
            next_state->add_constraint(sym::make_constraint_less(latest_global_timestamp, new_global_timestamp));
            next_state->set_latest_timestamp(default_global_clock, new_global_timestamp);
            next_state->bind_global_timestamp(_timestamp->name(), new_global_timestamp);
            
            next_state->append_history(new_global_timestamp);
            
            auto know_timestamp = program->new_timestamp();
            next_state->add_constraint(sym::make_constraint_less(know_timestamp, new_global_timestamp->clone()));
            auto know_event = program->know_event(new_global_timestamp->clone(), know_timestamp->clone());
            next_state->add_premise(know_event);
        } else {
            auto new_global_timestamp = program->new_timestamp();
            auto latest_global_timestamp = (sym::Number *)next_state->get_latest_timestamp(default_global_clock)->clone();
            next_state->add_constraint(sym::make_constraint_less(latest_global_timestamp, new_global_timestamp));
            next_state->set_latest_timestamp(default_global_clock, new_global_timestamp);

            auto new_local_timestamp = (sym::Number *) next_state->declare_timestamp(_timestamp->name(), _location)->clone();
            auto latest_local_timestamp = (sym::Number *)next_state->get_latest_timestamp(_clock)->clone();
            next_state->add_constraint(sym::make_constraint_less(latest_local_timestamp, new_local_timestamp));
            next_state->set_latest_timestamp(_clock, new_local_timestamp);
            next_state->bind_global_timestamp(_timestamp->name(), new_local_timestamp);
            
//            auto timestamp_pair = sym::make_pair(new_local_timestamp->clone(), new_global_timestamp->clone());
//            next_state->own_term(timestamp_pair);
//            next_state->append_history(timestamp_pair);
            next_state->append_history(new_local_timestamp);
            
            // add the relation between local and global clock
            if (program->shared_clock_rate()) {
                next_state->add_constraint(sym::make_constraint_equal(new_local_timestamp->clone(), sym::make_plus(new_global_timestamp->clone(), program->constant_clock_drift(_clock))));
            } else {
                next_state->add_constraint(sym::make_constraint_less_equal(new_local_timestamp->clone(), sym::make_plus(new_global_timestamp->clone(), program->maximum_clock_drift(_clock))));
                next_state->add_constraint(sym::make_constraint_less_equal(new_global_timestamp->clone(), sym::make_plus(new_local_timestamp->clone(), program->maximum_clock_drift(_clock))));
            }
            
            auto know_timestamp = program->new_timestamp();
            next_state->add_constraint(sym::make_constraint_less(know_timestamp, new_global_timestamp->clone()));
            auto know_event = program->know_event(new_local_timestamp->clone(), know_timestamp->clone());
            next_state->add_premise(know_event);
        }
        
        auto left = _left->evaluate(program, next_state);
        auto right = _right->evaluate(program, next_state);
        ASSERT(_next_process != nullptr);
        
        auto left_sort = left->sort();
        auto right_sort = right->sort();
        if ((left_sort == sym::TermSort::TERM_SORT_NUMBER || left_sort == sym::TERM_SORT_LINEAR_EXPRESSION) && (right_sort == sym::TermSort::TERM_SORT_NUMBER || right_sort == sym::TermSort::TERM_SORT_LINEAR_EXPRESSION)) {
            sym::Constraint *constraint = nullptr;
            if (_op == WaitType::WAIT_EQUAL) {
                constraint = sym::make_constraint_equal(left, right);
            } else if (_op == WaitType::WAIT_LESS) {
                constraint = sym::make_constraint_less(left, right);
            } else if (_op == WaitType::WAIT_LESS_EQUAL) {
                constraint = sym::make_constraint_less_equal(left, right);
            } else if (_op == WaitType::WAIT_GREATER) {
                constraint = sym::make_constraint_greater(left, right);
            } else {
                ASSERT(_op == WaitType::WAIT_GREATER_EQUAL);
                constraint = sym::make_constraint_greater_equal(left, right);
            }
            next_state->add_constraint(constraint);
            _next_process->execute(program, next_state, rules);
        } else {
            std::stringstream ss;
            ss << "@wait condition at " << _location << " is not linear";
            USER_ERROR(ss.str());
        }

        delete next_state;
    }

    void Wait::info(const size_t depth, std::ostream &os) const
    {
        os << com::tabs(process_tab_size, depth) << process_wait << process_space;
        _timestamp->info(os);
        os << process_space << process_until;
        _left->info(os);
        os << process_space << WaitTypeName[_op] << process_space;
        _right->info(os);
        os << process_endline << std::endl;
        _next_process->info(depth, os);
    }
}