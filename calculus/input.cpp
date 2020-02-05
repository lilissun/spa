//
//  input.cpp
//  calculus
//
//  Created by Li Li on 9/5/15.
//  Copyright (c) 2015 Lilissun. All rights reserved.
//

#include "input.h"

#include "common/iostream.h"
#include "common/debug.h"
#include "symbolic/term.h"
#include "symbolic/variable.h"
#include "symbolic/function.h"
#include "symbolic/tuple.h"
#include "symbolic/number.h"
#include "symbolic/event.h"
#include "symbolic/constraint.h"
#include "name.h"
#include "expression.h"
#include "tuple.h"
#include "state.h"
#include "program.h"

namespace cal {
    Input::~Input()
    {
        delete _identifier;
        delete _next_process;
    }
    
    void Input::execute(Program *program, const State *state, std::vector<tim::Rule *> &rules) const
    {
        State *next_state = state->clone();
        
        auto latest_timestamp = next_state->get_latest_timestamp(default_global_clock)->clone();
        auto current_timestamp = program->new_timestamp();
        next_state->add_constraint(sym::make_constraint_less(latest_timestamp, current_timestamp));
        
        next_state->set_latest_timestamp(default_global_clock, current_timestamp);
        
        auto input_timestamp = program->new_timestamp();
        auto constraint = sym::make_constraint_less(input_timestamp, current_timestamp->clone());
        next_state->add_constraint(constraint);

        auto identifier = _identifier->evaluate(program, next_state);
        auto know_event = program->know_event(identifier, input_timestamp->clone());
        next_state->add_premise(know_event);
        
        next_state->append_history(identifier);
        
        _next_process->execute(program, next_state, rules);
        delete next_state;
    }
    
    void Input::info(const size_t depth, std::ostream &os) const
    {
        os << com::tabs(process_tab_size, depth) << process_input << process_space;
        _identifier->info(os);
        os << process_endline << std::endl;
        _next_process->info(depth, os);
    }
}