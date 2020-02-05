//
//  tuple.h
//  calculus
//
//  Created by Li Li on 12/8/15.
//  Copyright (c) 2015 Lilissun. All rights reserved.
//

#ifndef __calculus__tuple__
#define __calculus__tuple__

#include <iostream>
#include <string>

#include "expression.h"

namespace sym {
    class Tuple;
}

namespace cal {
    class Tuple : public Expression {
    protected:
        std::vector<Expression *> _arguments; // owned

    public:
        Tuple(const std::vector<Expression *> &arguments, const com::Location &location)
        : Expression(ExpressionType::EXP_TUPLE, location), _arguments(arguments) {}
        ~Tuple();
        
    public:
        size_t size() const { return _arguments.size(); }
        const Expression * at(size_t index) const { return _arguments.at(index); }
        
    public:
        sym::Tuple * evaluate_tuple(const Program *program, State *state) const;
        sym::Tuple * evaluate_tuple(const State *state) const;
        
    public:
        virtual sym::Term * evaluate(const Program *program, State *state) const;
        virtual sym::Term * evaluate(const State *state) const;
        
    public:
        virtual void info(std::ostream &os) const;
    };
}

#endif /* defined(__calculus__tuple__) */
