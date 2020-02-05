//
//  name.h
//  calculus
//
//  Created by Li Li on 26/5/15.
//  Copyright (c) 2015 Lilissun. All rights reserved.
//

#ifndef __calculus__name__
#define __calculus__name__

#include <iostream>

#include "expression.h"

namespace cal {
    class Name : public Expression {
    protected:
        std::string _name;
        
    public:
        Name(const std::string &name, const com::Location &location) : Expression(EXP_NAME, location), _name(name) {}
        virtual ~Name() {}
        
    public:
        std::string name() const { return _name; }
        
    public:
        virtual sym::Term * evaluate(const Program *program, State *state) const;
        virtual sym::Term * evaluate(const State *state) const;
        
    public:
        virtual void info(std::ostream &os) const;
    };
}

#endif /* defined(__calculus__name__) */
