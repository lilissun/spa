//
//  function.h
//  calculus
//
//  Created by Li Li on 26/5/15.
//  Copyright (c) 2015 Lilissun. All rights reserved.
//

#ifndef __calculus__function__
#define __calculus__function__

#include <iostream>
#include <string>

#include "expression.h"
#include "symbolic/function.h"

namespace cal {
    class Tuple;
    class Function : public Expression {
    protected:
        sym::FunctionType _op;
        std::string _name;
        Tuple *_arguments; // owned
        
    public:
        Function(const std::string &name, Tuple *arguments, const com::Location &location) : Expression(EXP_FUNCTION, location), _op(sym::FunctionType::FUNCTION_USER_DEFINED), _name(name), _arguments(arguments) {}
        Function(sym::FunctionType op, Tuple *arguments, const com::Location &location) : Expression(EXP_FUNCTION, location), _op(op), _name(), _arguments(arguments) {}
        virtual ~Function();
        
    public:
        sym::FunctionType op() const { return _op; }
        const std::string & name() const { return _name; }
        const Tuple * arguments() const { return _arguments; }

    public:
        sym::Function * evaluate_function(const State *state) const;
        
    public:
        virtual sym::Term * evaluate(const Program *program, State *state) const;
        virtual sym::Term * evaluate(const State *state) const;
        
    public:
        virtual void info(std::ostream &os) const;
    };
}

#endif /* defined(__calculus__function__) */
