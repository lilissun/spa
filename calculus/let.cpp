//
//  let.cpp
//  calculus
//
//  Created by Li Li on 9/5/15.
//  Copyright (c) 2015 Lilissun. All rights reserved.
//

#include "let.h"

#include "common/iostream.h"
#include "common/debug.h"
#include "symbolic/term.h"
#include "symbolic/number.h"
#include "symbolic/variable.h"
#include "symbolic/number.h"
#include "expression.h"
#include "function.h"
#include "tuple.h"
#include "identifier.h"
#include "name.h"
#include "program.h"
#include "state.h"

namespace cal {
    Let::~Let()
    {
        delete _identifier;
        delete _function;
        delete _then_process;
        delete _else_process;
    }
    
    void Let::execute(Program *program, const State *state, std::vector<tim::Rule *> &rules) const
    {
        {
            State *next_state = state->clone();
            ASSERT(_then_process != nullptr);
            
            auto identifier = _identifier->evaluate(program, next_state);
            auto result = _function->evaluate(program, next_state);
            next_state->unify(result, identifier);
            
            _then_process->execute(program, next_state, rules);
            delete next_state;
        }
        
        if (_else_process != nullptr) {
            NOT_IMPL;
        }
    }
    
    void Let::info(const size_t depth, std::ostream &os) const
    {
        os << com::tabs(process_tab_size, depth) << process_let << process_space;
        _identifier->info(os);
        os << process_space << process_equal << process_space;
        _function->info(os);
        os << process_space;
        if (_else_process == nullptr) {
            os << process_then << std::endl;
            _then_process->info(depth, os);
        } else {
            os << process_left_bracket << std::endl;
            _then_process->info(depth + 1, os);
            os << com::tabs(process_tab_size, depth) << process_right_bracket << process_space << process_else << process_space << process_left_bracket << std::endl;
            _else_process->info(depth + 1, os);
            os << com::tabs(process_tab_size, depth) << process_right_bracket << std::endl;
        }
    }
}