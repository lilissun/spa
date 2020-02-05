//
//  output.cpp
//  calculus
//
//  Created by Li Li on 9/5/15.
//  Copyright (c) 2015 Lilissun. All rights reserved.
//

#include "output.h"

#include "common/iostream.h"
#include "common/debug.h"
#include "symbolic/term.h"
#include "symbolic/number.h"
#include "symbolic/constant.h"
#include "symbolic/function.h"
#include "symbolic/tuple.h"
#include "symbolic/constraint.h"
#include "expression.h"
#include "identifier.h"
#include "state.h"
#include "program.h"

namespace cal {
    Output::~Output()
    {
        delete _term;
        delete _next_process;
    }
    
    void Output::execute(Program *program, const State *state, std::vector<tim::Rule *> &rules) const
    {
        State *next_state = state->clone();
        
        auto output_timestamp = program->new_timestamp();
        auto latest_timestamp = next_state->get_latest_timestamp(default_global_clock)->clone();
        auto network_delay = program->network_delay();
        auto constraint = sym::make_constraint_greater_equal(sym::make_minus(output_timestamp, latest_timestamp), network_delay);
        next_state->add_constraint(constraint);
        
        auto term = _term->evaluate(program, next_state);
        auto know_event = program->know_event(term, output_timestamp->clone());
        rules.push_back(next_state->construct_rule(know_event));
        
        _next_process->execute(program, next_state, rules);
        delete next_state;
    }
    
    void Output::info(const size_t depth, std::ostream &os) const
    {
        os << com::tabs(process_tab_size, depth) << process_output << process_space;
        _term->info(os);
        os << process_endline << std::endl;
        _next_process->info(depth, os);
    }
}