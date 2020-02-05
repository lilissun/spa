//
//  expression.h
//  calculus
//
//  Created by Li Li on 25/5/15.
//  Copyright (c) 2015 Lilissun. All rights reserved.
//

#ifndef __calculus__expression__
#define __calculus__expression__

#include <iostream>
#include <vector>
#include <string>

#include "common/location.h"
#include "symbolic/model.h"

namespace sym {
    class Term;
}

namespace cal {
    const std::string expression_left_para = "(";
    const std::string expression_right_para = ")";
    const std::string expression_left_comma = ",";
    const std::string expression_space = " ";
    const std::string expression_variable = "@variable";
    const std::string expression_timestamp = "@timestamp";
    
    enum ExpressionType {
        EXP_NAME,
        EXP_TUPLE, 
        EXP_FUNCTION,
        EXP_VALUE,
        EXP_IDENTIFIER,
        EXP_TYPE_END,
    };
        
    class Program;
    class State;
    class Expression : public sym::Model {
    protected:
        ExpressionType _type;
        com::Location _location;
        
    public:
        Expression(ExpressionType type, const com::Location &location) : _type(type), _location(location) {}
        virtual ~Expression() {}
        
    public:
        virtual sym::Term * evaluate(const Program *program, State *state) const = 0;
        virtual sym::Term * evaluate(const State *state) const = 0;
        
    public:
        virtual void info(std::ostream &os) const = 0;
    };
}

#endif /* defined(__calculus__expression__) */
