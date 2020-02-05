//
//  constraint.h
//  calculus
//
//  Created by Li Li on 25/11/15.
//  Copyright © 2015 Lilissun. All rights reserved.
//

#ifndef constraint_hpp
#define constraint_hpp

#include <iostream>

#include "symbolic/constraint.h"

#include "tuple.h"
#include "name.h"

namespace sym {
    class Constraint;
}

namespace cal {
    class Constraint : public sym::Model {
    protected:
        Expression *_left;
        sym::ConstraintType _type;
        Expression *_right;
        
    public:
        Constraint(Expression *left, sym::ConstraintType type, Expression *right) : _left(left), _type(type), _right(right) {}
        virtual ~Constraint();
        
    public:
        sym::ConstraintType type() const { return _type; }
        sym::Constraint * evaluate(const State *state) const;
        
    public:
        void info(std::ostream &os) const;
    };
}

#endif /* constraint_h */
