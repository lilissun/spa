//
//  unique.cpp
//  calculus
//
//  Created by Li Li on 29/5/15.
//  Copyright (c) 2015 Lilissun. All rights reserved.
//

#include "unique.h"

#include "common/iostream.h"
#include "common/debug.h"
#include "symbolic/nonce.h"
#include "symbolic/function.h"
#include "symbolic/number.h"
#include "symbolic/name.h"
#include "symbolic/tuple.h"
#include "symbolic/event.h"
#include "symbolic/pair.h"
#include "symbolic/list.h"
#include "context.h"
#include "program.h"
#include "expression.h"
#include "name.h"
#include "state.h"

namespace cal {
    Unique::~Unique()
    {
        delete _expression;
        delete _database;
        delete _next_process;
    }
    
    void Unique::execute(Program *program, const State *state, std::vector<tim::Rule *> &rules) const
    {
        State *next_state = state->clone();
        auto term = _expression->evaluate(program, next_state);
        
        auto term_location = program->unique_location();
        auto unique_event = program->unique_event(term, term_location, sym::make_list(next_state->get_history()->clone()));
        next_state->add_premise(unique_event);

        auto unique_pair = sym::make_pair(term->clone(), term_location->clone());
        next_state->own_term(unique_pair);
        next_state->set_identity(unique_pair);

        _next_process->execute(program, next_state, rules);
        delete next_state;
    }
    
    void Unique::info(const size_t depth, std::ostream &os) const
    {
        os << com::tabs(process_tab_size, depth) << process_unique << process_space << _expression << process_space << process_in << process_space << _database << process_endline << std::endl;
        _next_process->info(depth, os);
    }
}