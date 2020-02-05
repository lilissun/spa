//
//  open.cpp
//  calculus
//
//  Created by Li Li on 13/8/15.
//  Copyright (c) 2015 Lilissun. All rights reserved.
//

#include "open.h"

#include "common/iostream.h"
#include "common/debug.h"
#include "symbolic/term.h"
#include "symbolic/tuple.h"
#include "symbolic/nonce.h"
#include "symbolic/name.h"
#include "expression.h"
#include "tuple.h"
#include "name.h"
#include "state.h"
#include "program.h"

namespace cal {
    Open::~Open()
    {
        delete _term;
        delete _next_process;
    }
    
    void Open::execute(Program *program, const State *state, std::vector<tim::Rule *> &rules) const
    {
        State *next_state = state->clone();
        auto term = _term->evaluate(program, next_state);
        next_state->add_premise(program->open_event(term));
        _next_process->execute(program, next_state, rules);
        delete next_state;
    }
    
    void Open::info(const size_t depth, std::ostream &os) const
    {
        os << com::tabs(process_tab_size, depth) << process_open << process_space;
        _term->info(os);
        os << process_endline << std::endl;
        _next_process->info(depth, os);
    }
}