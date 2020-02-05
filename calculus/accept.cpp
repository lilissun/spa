//
//  accept.cpp
//  calculus
//
//  Created by Li Li on 9/5/15.
//  Copyright (c) 2015 Lilissun. All rights reserved.
//

#include "accept.h"

#include "common/iostream.h"
#include "common/debug.h"
#include "symbolic/term.h"
#include "symbolic/tuple.h"
#include "symbolic/number.h"
#include "symbolic/nonce.h"
#include "symbolic/name.h"
#include "expression.h"
#include "tuple.h"
#include "name.h"
#include "state.h"
#include "program.h"

namespace cal {
    Accept::~Accept()
    {
        delete _name;
        delete _arguments;
        delete _timestamp;
        delete _next_process;
    }
    
    void Accept::execute(Program *program, const State *state, std::vector<tim::Rule *> &rules) const
    {
        State *next_state = state->clone();
        auto arguments = _arguments->evaluate_tuple(program, next_state);
        auto timestamp = next_state->get_global_timestamp(_timestamp->name())->clone();
        ASSERT(timestamp->type() == sym::TermType::TERM_NUMBER);
        
        auto accept_event = program->accept_event(_name->name(), next_state->get_identity()->clone(), arguments, timestamp);
        rules.push_back(next_state->construct_rule(accept_event));

        _next_process->execute(program, next_state, rules);
        delete next_state;
    }
    
    void Accept::info(const size_t depth, std::ostream &os) const
    {
        os << com::tabs(process_tab_size, depth) << process_engage << process_space;
        os << process_accept << process_space;
        _name->info(os);
        _arguments->info(os);
        os << process_at;
        _timestamp->info(os);
        os << process_endline << std::endl;
        _next_process->info(depth, os);
    }
}