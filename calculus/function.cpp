//
//  function.cpp
//  calculus
//
//  Created by Li Li on 26/5/15.
//  Copyright (c) 2015 Lilissun. All rights reserved.
//

#include "function.h"

#include "common/debug.h"
#include "explicit/indexer.h"
#include "symbolic/function.h"
#include "symbolic/tuple.h"
#include "tuple.h"
#include "state.h"
#include "context.h"
#include "program.h"

namespace cal {
    Function::~Function()
    {
        delete _arguments;
    }
    
    sym::Function * Function::evaluate_function(const State *state) const
    {
        if (_op == sym::FunctionType::FUNCTION_USER_DEFINED) {
            return sym::make_function(_name, _arguments->evaluate_tuple(state), state->context()->namer());
        } else if (_op == sym::FunctionType::FUNCTION_PLUS) {
            ASSERT(_arguments->size() == 2);
            return sym::make_plus(_arguments->at(0)->evaluate(state), _arguments->at(1)->evaluate(state));
        } else if (_op == sym::FunctionType::FUNCTION_MULTIPLY) {
            ASSERT(_arguments->size() == 2);
            return sym::make_multiply(_arguments->at(0)->evaluate(state), _arguments->at(1)->evaluate(state));
        } else if (_op == sym::FunctionType::FUNCTION_MINUS) {
            ASSERT(_arguments->size() == 2);
            return sym::make_minus(_arguments->at(0)->evaluate(state), _arguments->at(1)->evaluate(state));
        } else {
            ASSERT(_op == sym::FunctionType::FUNCTION_NEGATIVE);
            ASSERT(_arguments->size() == 1);
            return sym::make_negative(_arguments->at(0)->evaluate(state));
        }
    }
    
    sym::Term * Function::evaluate(const Program *program, State *state) const
    {
        if (_op == sym::FunctionType::FUNCTION_USER_DEFINED) {
            auto function = sym::make_function(_name, _arguments->evaluate_tuple(program, state), state->context()->namer());
            
            auto application = program->function(_name);
            if (application.second == nullptr) {
                state->unify(application.first, function);
                return function->clone();
            } else {
                state->unify(application.first, function);
                return application.second;
            }
        } else if (_op == sym::FunctionType::FUNCTION_PLUS) {
            ASSERT(_arguments->size() == 2);
            return sym::make_plus(_arguments->at(0)->evaluate(program, state), _arguments->at(1)->evaluate(program, state));
        } else if (_op == sym::FunctionType::FUNCTION_MULTIPLY) {
            ASSERT(_arguments->size() == 2);
            return sym::make_multiply(_arguments->at(0)->evaluate(program, state), _arguments->at(1)->evaluate(program, state));
        } else if (_op == sym::FunctionType::FUNCTION_MINUS) {
            ASSERT(_arguments->size() == 2);
            return sym::make_minus(_arguments->at(0)->evaluate(program, state), _arguments->at(1)->evaluate(program, state));
        } else {
            ASSERT(_op == sym::FunctionType::FUNCTION_NEGATIVE);
            ASSERT(_arguments->size() == 1);
            return sym::make_negative(_arguments->at(0)->evaluate(program, state));
        }
    }
    
    sym::Term * Function::evaluate(const State *state) const
    {
        return evaluate_function(state);
    }

    void Function::info(std::ostream &os) const
    {
        if (_op == sym::FunctionType::FUNCTION_USER_DEFINED) {
            os << _name;
            _arguments->info(os);
        } else if (sym::FunctionTypeInfo[_op].size() == 1) {
            ASSERT(_arguments->size() == 1);
            os << sym::FunctionTypeInfo[_op].symbol() << expression_space;
            _arguments->at(0)->info(os);
        } else if (sym::FunctionTypeInfo[_op].size() == 2) {
            ASSERT(_arguments->size() == 2);
            _arguments->at(0)->info(os);
            os << expression_space << sym::FunctionTypeInfo[_op].symbol() << expression_space;
            _arguments->at(1)->info(os);
        } else {
            SYSTEM_ERROR("not supported function operator");
        }
    }
}