//
//  call.cpp
//  calculus
//
//  Created by Li Li on 27/5/15.
//  Copyright (c) 2015 Lilissun. All rights reserved.
//

#include "call.h"

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
    Call::~Call()
    {
        delete _name;
        delete _arguments;
    }
    
    void Call::execute(Program *program, const State *state, std::vector<tim::Rule *> &rules) const
    {
        auto next_state = state->clone();
        auto call_arguments = _arguments->evaluate_tuple(program, next_state);
        auto new_state = next_state->reset(program->init_state());
        auto process = program->process(_name->name());
        ASSERT(process.first->size() == _arguments->size());
        auto process_arguments = process.first->evaluate_tuple(program, new_state);
        new_state->unify(call_arguments, process_arguments);
        process.second->execute(program, new_state, rules);
        delete new_state;
        delete next_state;
    }
    
    void Call::info(const size_t depth, std::ostream &os) const
    {
        os << com::tabs(process_tab_size, depth) << process_invoke << process_space;
        _name->info(os);
        _arguments->info(os);
        os << process_endline << std::endl;
    }
}