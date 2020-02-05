//
//  secrecy.cpp
//  calculus
//
//  Created by Li Li on 9/5/15.
//  Copyright (c) 2015 Lilissun. All rights reserved.
//

#include "secrecy.h"

#include "common/iostream.h"
#include "common/debug.h"
#include "symbolic/term.h"
#include "symbolic/tuple.h"
#include "symbolic/nonce.h"
#include "symbolic/number.h"
#include "symbolic/name.h"
#include "symbolic/constraint.h"
#include "expression.h"
#include "tuple.h"
#include "name.h"
#include "state.h"
#include "program.h"

namespace cal {
    Secrecy::~Secrecy()
    {
        delete _term;
        delete _next_process;
    }
    
    void Secrecy::execute(Program *program, const State *state, std::vector<tim::Rule *> &rules) const
    {
        auto next_state = state->clone();
        _next_process->execute(program, next_state, rules);
        
        // actually they are different processes, reuse it.
        auto current_timestamp = program->new_timestamp();
        next_state->set_latest_timestamp(default_global_clock, current_timestamp);
        auto input_timestamp = program->new_timestamp();
        auto constraint = sym::make_constraint_less(input_timestamp, current_timestamp);
        next_state->add_constraint(constraint);

        auto term = _term->evaluate(program, next_state);
        auto know_event = program->know_event(term, input_timestamp->clone());
        next_state->add_premise(know_event);
        next_state->append_history(term);
        rules.push_back(next_state->construct_rule(program->leak_event(term->clone())));
        
        delete next_state;
    }
    
    void Secrecy::info(const size_t depth, std::ostream &os) const
    {
        os << com::tabs(process_tab_size, depth) << process_secrecy << process_space;
        _term->info(os);
        os << process_endline << std::endl;
        _next_process->info(depth, os);
    }
}