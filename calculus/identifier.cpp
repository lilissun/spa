//
//  identifier.cpp
//  calculus
//
//  Created by Li Li on 26/5/15.
//  Copyright (c) 2015 Lilissun. All rights reserved.
//

#include "identifier.h"

#include "common/debug.h"
#include "common/runtime.h"
#include "explicit/indexer.h"
#include "symbolic/function.h"
#include "symbolic/variable.h"
#include "symbolic/number.h"
#include "state.h"
#include "context.h"

namespace cal {
    sym::Term * Identifier::evaluate(const Program *program, State *state) const
    {
        if (_variable) {
            return state->declare_variable(_name, _location)->clone();
        } else {
            return state->declare_timestamp(_name, _location)->clone();
        }
    }
    
    sym::Term * Identifier::evaluate(const State *state) const
    {
        SYSTEM_ERROR("should not be reachable");
    }
    
    void Identifier::info(std::ostream &os) const
    {
        os << (_variable ? expression_variable : expression_timestamp);
        os << expression_space << _name;
    }
}