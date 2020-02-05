//
//  identifier.h
//  calculus
//
//  Created by Li Li on 26/5/15.
//  Copyright (c) 2015 Lilissun. All rights reserved.
//

#ifndef __calculus__identifier__
#define __calculus__identifier__

#include <iostream>

#include "expression.h"

namespace cal {
    class Identifier : public Expression {
    protected:
        bool _variable;
        std::string _name;
        
    public:
        Identifier(bool variable, const std::string &name, const com::Location &location) : Expression(EXP_IDENTIFIER, location), _variable(variable), _name(name) {}
        virtual ~Identifier() {}
        
    public:
        bool variable() const { return _variable; }
        std::string name() const { return _name; }
        
    public:
        virtual sym::Term * evaluate(const Program *program, State *state) const;
        virtual sym::Term * evaluate(const State *state) const;
        
    public:
        virtual void info(std::ostream &os) const;
    };
}

#endif /* defined(__calculus__identifier__) */
