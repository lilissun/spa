//
//  value.h
//  calculus
//
//  Created by Li Li on 26/5/15.
//  Copyright (c) 2015 Lilissun. All rights reserved.
//

#ifndef __calculus__value__
#define __calculus__value__

#include <iostream>

#include "expression.h"
#include "explicit/value.h"

namespace cal {
    class Value : public Expression {
    protected:
        int _value;
        
    public:
        Value(int value, const com::Location &location) : Expression(EXP_VALUE, location), _value(value) {}
        virtual ~Value() {}
        
    public:
        int value() const { return _value; }
        
    public:
        virtual sym::Term * evaluate(const Program *program, State *state) const;
        virtual sym::Term * evaluate(const State *state) const;
        
    public:
        virtual void info(std::ostream &os) const;
    };
}

#endif /* defined(__calculus__value__) */
