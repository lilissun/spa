//
//  value.cpp
//  calculus
//
//  Created by Li Li on 26/5/15.
//  Copyright (c) 2015 Lilissun. All rights reserved.
//

#include "value.h"

#include "common/debug.h"
#include "explicit/indexer.h"
#include "symbolic/value.h"
#include "state.h"
#include "context.h"

namespace cal {
    sym::Term * Value::evaluate(const Program *program, State *state) const
    {
        return sym::make_value(_value);
    }
        
    sym::Term * Value::evaluate(const State *state) const
    {
        return sym::make_value(_value);
    }

    void Value::info(std::ostream &os) const
    {
        os << _value;
    }
}