//
//  replication.cpp
//  calculus
//
//  Created by Li Li on 9/5/15.
//  Copyright (c) 2015 Lilissun. All rights reserved.
//

#include "replication.h"

#include "common/iostream.h"
#include "common/debug.h"
#include "state.h"
#include "null.h"

namespace cal {
    void Replication::execute(Program *program, const State *state, std::vector<tim::Rule *> &rules) const
    {
        State *next_state = state->clone();
        
        next_state->init_history();
        
        _next_process->execute(program, next_state, rules);
        delete next_state;
    }
    
    void Replication::info(const size_t depth, std::ostream &os) const
    {
        os << com::tabs(process_tab_size, depth) << process_replication << process_left_bracket << std::endl;
        _next_process->info(depth + 1, os);
        os << com::tabs(process_tab_size, depth) << process_right_bracket << std::endl;
    }
}