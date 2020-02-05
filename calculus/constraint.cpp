//
//  constraint.cpp
//  calculus
//
//  Created by Li Li on 25/11/15.
//  Copyright © 2015 Lilissun. All rights reserved.
//

#include "constraint.h"

#include "expression.h"

namespace cal {
    Constraint::~Constraint()
    {
        delete _left;
        delete _right;
    }
    
    sym::Constraint * Constraint::evaluate(const State *state) const
    {
        return sym::make_constraint(_left->evaluate(state), _type, _right->evaluate(state));
    }
    
    void Constraint::info(std::ostream &os) const
    {
        _left->info(os);
        os << expression_space << sym::ConstraintTypeName[_type] << expression_space;
        _right->info(os);
    }
}