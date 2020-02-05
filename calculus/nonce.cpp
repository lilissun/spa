//
//  nonce.cpp
//  calculus
//
//  Created by Li Li on 9/5/15.
//  Copyright (c) 2015 Lilissun. All rights reserved.
//

#include "nonce.h"

#include "common/iostream.h"
#include "common/debug.h"
#include "symbolic/nonce.h"
#include "symbolic/function.h"
#include "symbolic/name.h"
#include "symbolic/tuple.h"
#include "symbolic/event.h"
#include "symbolic/list.h"
#include "context.h"
#include "program.h"
#include "expression.h"
#include "name.h"
#include "state.h"

namespace cal {
    Nonce::~Nonce()
    {
        delete _nonce;
        delete _next_process;
    }
    
    void Nonce::execute(Program *program, const State *state, std::vector<tim::Rule *> &rules) const
    {
        State *next_state = state->clone();
        
        auto nonce = (sym::Nonce *)next_state->declare_nonce(_nonce->name(), _location)->clone();
        
        auto location = program->unique_location();
        auto new_event = program->new_event(nonce, location);
        next_state->add_premise(new_event);
        
        auto unique_event = program->unique_event(nonce->clone(), location->clone(), sym::make_list(next_state->get_history()->clone()));
        next_state->add_premise(unique_event);
        
        next_state->append_history(nonce);
        next_state->set_identity(nonce);
        
        _next_process->execute(program, next_state, rules);
        delete next_state;
    }
    
    void Nonce::info(const size_t depth, std::ostream &os) const
    {
        os << com::tabs(process_tab_size, depth) << process_nonce << process_space << _nonce;
        os << process_endline << std::endl;
        _next_process->info(depth, os);
    }
}