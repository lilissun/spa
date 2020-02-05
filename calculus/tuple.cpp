//
//  tuple.cpp
//  calculus
//
//  Created by Li Li on 12/8/15.
//  Copyright (c) 2015 Lilissun. All rights reserved.
//

#include "tuple.h"

#include "symbolic/tuple.h"

namespace cal {
    Tuple::~Tuple()
    {
        for (auto i = _arguments.begin(); i != _arguments.end(); ++i) {
            delete *i;
        }
    }

    sym::Tuple * Tuple::evaluate_tuple(const Program *program, State *state) const
    {
        std::vector<sym::Term *> arguments;
        for (auto i = _arguments.begin(); i != _arguments.end(); ++i) {
            arguments.push_back((*i)->evaluate(program, state));
        }
        return sym::make_tuple(arguments);
    }
    
    sym::Tuple * Tuple::evaluate_tuple(const State *state) const
    {
        std::vector<sym::Term *> arguments;
        for (auto i = _arguments.begin(); i != _arguments.end(); ++i) {
            arguments.push_back((*i)->evaluate(state));
        }
        return sym::make_tuple(arguments);
    }
    
    sym::Term * Tuple::evaluate(const Program *program, State *state) const
    {
        return evaluate_tuple(program, state);
    }
    
    sym::Term * Tuple::evaluate(const State *state) const
    {
        return evaluate_tuple(state);
    }
    
    void Tuple::info(std::ostream &os) const
    {
        os << expression_left_para;
        for (auto i = _arguments.begin(); i != _arguments.end(); ++i) {
            if (i != _arguments.begin()) {
                os << expression_left_comma << expression_space;
            }
            (*i)->info(os);
        }
        os << expression_right_para;
    }
}