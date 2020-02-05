//
//  parallel.cpp
//  calculus
//
//  Created by Li Li on 9/5/15.
//  Copyright (c) 2015 Lilissun. All rights reserved.
//

#include "parallel.h"

#include "common/iostream.h"
#include "common/debug.h"
#include "symbolic/variable.h"
#include "symbolic/tuple.h"
#include "symbolic/name.h"
#include "state.h"
#include "null.h"
#include "program.h"

namespace cal {
    Parallel::~Parallel()
    {
        for (auto i = _next_processes.begin(); i != _next_processes.end(); ++i) {
            delete (*i);
        }
    }
    
    void Parallel::execute(Program *program, const State *state, std::vector<tim::Rule *> &rules) const
    {
        auto next_state = state->clone();
        
        std::vector<sym::Term *> tuple_arguments;
        for (auto i = 0; i != _next_processes.size(); ++i) {
            tuple_arguments.push_back(program->new_variable());
        }
        auto tuple = sym::make_tuple(tuple_arguments);
        next_state->append_history(tuple);
        
        auto identity = next_state->get_raw_identity();
        for (auto i = 0; i != _next_processes.size(); ++i) {
            next_state->set_history_tail(tuple->at(i));
            if (identity != nullptr) {
                auto new_identity = sym::make_tuple({identity->clone(), program->unique_location()});
                next_state->own_term(new_identity);
                next_state->set_identity(new_identity);
            }
            _next_processes.at(i)->execute(program, next_state, rules);
        }
        
        delete next_state;
    }
    
    void Parallel::info(const size_t depth, std::ostream &os) const
    {
        ASSERT(_next_processes.size() >= 2);
        os << com::tabs(process_tab_size, depth) << process_parallel << process_space << process_left_bracket << std::endl;
        for (auto i = _next_processes.begin(); i != _next_processes.end(); ++i) {
            (*i)->info(depth + 1, os);
            if (std::next(i) != _next_processes.end()) {
                os << com::tabs(process_tab_size, depth) << process_bar << std::endl;
            } else {
                os << com::tabs(process_tab_size, depth) << process_right_bracket << std::endl;
            }
        }
    }
}