//
//  name.cpp
//  calculus
//
//  Created by Li Li on 26/5/15.
//  Copyright (c) 2015 Lilissun. All rights reserved.
//

#include "name.h"

#include "common/debug.h"
#include "explicit/indexer.h"
#include "symbolic/function.h"
#include "symbolic/variable.h"
#include "state.h"
#include "context.h"

namespace cal {
    sym::Term * Name::evaluate(const Program *program, State *state) const
    {
        return state->get_term(_name, _location)->clone();
    }
    
    sym::Term * Name::evaluate(const State *state) const
    {
        return state->get_term(_name, _location)->clone();
    }
    
    void Name::info(std::ostream &os) const
    {
        os << _name;
    }
}