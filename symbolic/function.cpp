//
//  function.cpp
//  symbolic
//
//  Created by Li Li on 22/8/13.
//  Copyright (c) 2013 Lilissun. All rights reserved.
//

#include "function.h"

#include "common/debug.h"
#include "common/runtime.h"
#include "explicit/namer.h"
#include "explicit/renamer.h"
#include "explicit/value.h"
#include "unifier.h"
#include "matcher.h"
#include "tuple.h"

namespace sym {
    bool FunctionPattern::match(const Term *term, const Unifier &unifier) const
    {
        if (term->type() != TermType::TERM_FUNCTION) {
            return false;
        }
        
        auto function = (const Function *)term;
        ASSERT(function->op() == FunctionType::FUNCTION_USER_DEFINED);
        if (function->name() != _name) {
            return false;
        }
        
        return function->arguments()->match(_arguments, unifier);
    }

    void FunctionPattern::info(std::ostream &os) const
    {
        os << _name;
        _arguments->info(os);
    }
    
    FunctionPattern * make_function_pattern(const std::string &name, TuplePattern *arguments, emc::Namer &namer)
    {
        return new FunctionPattern(namer.constant(name), arguments);
    }

    FunctionPattern * make_negative_pattern(Pattern *term)
    {
        return new FunctionPattern(FunctionType::FUNCTION_NEGATIVE, make_tuple_pattern({term}));
    }

    FunctionPattern * make_plus_pattern(Pattern *left, Pattern *right)
    {
        return new FunctionPattern(FunctionType::FUNCTION_PLUS, make_tuple_pattern({left, right}));
    }

    FunctionPattern * make_minus_pattern(Pattern *left, Pattern *right)
    {
        return new FunctionPattern(FunctionType::FUNCTION_MINUS, make_tuple_pattern({left, right}));
    }

    FunctionPattern * make_multiply_pattern(Pattern *left, Pattern *right)
    {
        return new FunctionPattern(FunctionType::FUNCTION_MULTIPLY, make_tuple_pattern({left, right}));
    }

    FunctionPattern * make_divide_pattern(Pattern *left, Pattern *right)
    {
        return new FunctionPattern(FunctionType::FUNCTION_DIVIDE, make_tuple_pattern({left, right}));
    }
    
    Function::Function(const emc::Name &name, Tuple *arguments)
    : Term(TermType::TERM_FUNCTION)
    , _op(FunctionType::FUNCTION_USER_DEFINED)
    , _name(name)
    , _arguments(arguments)
    {
        ASSERT(name.id() != -1);
    }
    
    Function::Function(FunctionType op, Tuple *arguments)
    : Term(TermType::TERM_FUNCTION)
    , _op(op)
    , _name(emc::INVALID_NAME)
    , _arguments(arguments)
    {
        ASSERT(_op != FunctionType::FUNCTION_USER_DEFINED);
        ASSERT(arguments->size() == FunctionTypeInfo[_op].size());
    }
    
    Function::~Function()
    {
        delete _arguments;
    }
    
    TermSort Function::_sort() const
    {
        if (_op == FunctionType::FUNCTION_USER_DEFINED) {
            return _arguments->sort();
        }
        ASSERT(_arguments->size() == FunctionTypeInfo[_op].size());
        if (_op == FunctionType::FUNCTION_NEGATIVE) {
            TermSort term_sort = _arguments->at(0)->_sort();
            if (term_sort == TermSort::TERM_SORT_VALUE) {
                return TermSort::TERM_SORT_VALUE;
            } else if (term_sort == TermSort::TERM_SORT_NUMBER) {
                return TermSort::TERM_SORT_LINEAR_EXPRESSION;
            } else if (term_sort == TermSort::TERM_SORT_LINEAR_EXPRESSION) {
                return TermSort::TERM_SORT_LINEAR_EXPRESSION;
            } else {
                return TermSort::TERM_SORT_COMPLEX;
            }
        } else if (_op == FunctionType::FUNCTION_PLUS || _op == FunctionType::FUNCTION_MINUS) {
            TermSort first_sort = _arguments->at(0)->_sort();
            TermSort second_sort = _arguments->at(1)->_sort();
            // either the arguments are values
            if (first_sort == TermSort::TERM_SORT_VALUE && second_sort == TermSort::TERM_SORT_VALUE) {
                return TermSort::TERM_SORT_VALUE;
            }
            // either the arguments are linear expressions
            // TERM_SORT_VALUE +/- TERM_SORT_VALUE is impossible at this point
            if (first_sort == TermSort::TERM_SORT_LINEAR_EXPRESSION || first_sort == TermSort::TERM_SORT_NUMBER || first_sort == TermSort::TERM_SORT_VALUE) {
                if (second_sort == TermSort::TERM_SORT_LINEAR_EXPRESSION || second_sort == TermSort::TERM_SORT_NUMBER || second_sort == TermSort::TERM_SORT_VALUE) {
                    return TermSort::TERM_SORT_LINEAR_EXPRESSION;
                }
            }
            return TermSort::TERM_SORT_COMPLEX;
        } else if (_op == FunctionType::FUNCTION_MULTIPLY) {
            ASSERT(_arguments->size() == 2);
            TermSort first_sort = _arguments->at(0)->_sort();
            TermSort second_sort = _arguments->at(1)->_sort();
            // either the arguments are values
            if (first_sort == TermSort::TERM_SORT_VALUE && second_sort == TermSort::TERM_SORT_VALUE) {
                return TermSort::TERM_SORT_VALUE;
            }
            // either one of the arguments is a linear expression
            // and the other is a simple value
            // TERM_SORT_VALUE * TERM_SORT_VALUE is impossible at this point
            if (first_sort == TermSort::TERM_SORT_LINEAR_EXPRESSION || first_sort == TermSort::TERM_SORT_NUMBER) {
                if (second_sort == TermSort::TERM_SORT_VALUE) {
                    return TermSort::TERM_SORT_LINEAR_EXPRESSION;
                }
            }
            if (first_sort == TermSort::TERM_SORT_VALUE) {
                if (second_sort == TermSort::TERM_SORT_LINEAR_EXPRESSION || second_sort == TermSort::TERM_SORT_NUMBER) {
                    return TermSort::TERM_SORT_LINEAR_EXPRESSION;
                }
            }
            return TermSort::TERM_SORT_COMPLEX;
        } else {
            return TermSort::TERM_SORT_COMPLEX;
        }
    }
    
    void Function::unify(const Term *term, Unifier &unifier) const
    {
        ASSERT(_op == FunctionType::FUNCTION_USER_DEFINED);
        
        if (term->type() == TermType::TERM_FUNCTION) {
            const Function *function = (const Function *)term;
            ASSERT(function->_op == FunctionType::FUNCTION_USER_DEFINED);
            if (function->name() != _name) {
                throw UnifyFailed();
            }
            _arguments->unify(function->_arguments, unifier);
        } else if (term->type() == TermType::TERM_VARIABLE) {
            term->unify(this, unifier);
        } else if (term->type() == TermType::TERM_CONSTANT) {
            NOT_IMPL;
        } else if (term->type() == TermType::TERM_VALUE) {
            NOT_IMPL;
        } else {
            throw UnifyFailed();
        }
    }
    
    void Function::unify_to(const Term *term, Unifier &unifier) const
    {
        ASSERT(_op == FunctionType::FUNCTION_USER_DEFINED);

        if (term->type() == TermType::TERM_FUNCTION) {
            const Function *function = (const Function *)term;
            ASSERT(function->_op == FunctionType::FUNCTION_USER_DEFINED);
            if (function->name() != _name) {
                throw UnifyFailed();
            }
            _arguments->unify_to(function->_arguments, unifier);
        } else if (term->type() == TermType::TERM_CONSTANT) {
            NOT_IMPL;
        } else if (term->type() == TermType::TERM_VALUE) {
            NOT_IMPL;
        } else {
            throw UnifyFailed();
        }
    }
    
    bool Function::match(const Pattern *pattern, const Unifier &unifier) const
    {
        ASSERT(_op == FunctionType::FUNCTION_USER_DEFINED);
        return pattern == nullptr || pattern->match(this, unifier);
    }
    
    Term * Function::clone() const
    {
        if (_op == FunctionType::FUNCTION_USER_DEFINED) {
            return new Function(_name, _arguments->tuple_clone());
        } else {
            return new Function(_op, _arguments->tuple_clone());
        }
    }
    
    Term * Function::map(const Unifier &unifier) const
    {
        if (_op == FunctionType::FUNCTION_USER_DEFINED) {
            return new Function(_name, _arguments->tuple_map(unifier));
        } else {
            return new Function(_op, _arguments->tuple_map(unifier));
        }
    }
    
    Term * Function::rename(emc::Renamer &renamer, emc::Namer &namer)
    {
        _arguments->rename(renamer, namer);
        return this;
    }
    
    size_t Function::depth(const emc::Name &name, const Unifier &unifier) const
    {
        return _arguments->depth(name, unifier);
    }
    
    size_t Function::weight() const
    {
        return TermType::TERM_FUNCTION + _arguments->weight();
    }

    void Function::subnames(unsigned long mask, const Unifier &unifier, std::set<emc::Name> &names) const
    {
        if (mask & TermType::TERM_FUNCTION) {
            names.insert(_name);
        }
        _arguments->subnames(mask, unifier, names);
    }
    
    void Function::subnames(unsigned long mask, std::set<emc::Name> &names) const
    {
        if (mask & TermType::TERM_FUNCTION) {
            names.insert(_name);
        }
        _arguments->subnames(mask, names);
    }
        
    void Function::signature(const Unifier &unifier, std::ostream &os) const
    {
        ASSERT(_op == FunctionType::FUNCTION_USER_DEFINED);
        os << _name.id();
        _arguments->signature(unifier, os);
    }
    
    void Function::signature(std::ostream &os) const
    {
        ASSERT(_op == FunctionType::FUNCTION_USER_DEFINED);
        os << _name.id();
        _arguments->signature(os);
    }

    emc::Value Function::evaluate(const emc::Valuator &valuator) const
    {
        NOT_IMPL;
    }
    
    bool term_has_higher_function_priority(const Term *term, FunctionPriority priority)
    {
        if (term->type() != TermType::TERM_FUNCTION) {
            return FunctionTypeInfo[FunctionType::FUNCTION_USER_DEFINED].priority() > priority;
        } else {
            return FunctionTypeInfo[((Function *)term)->op()].priority() > priority;
        }
    }
    
    bool term_has_lower_function_priority(const Term *term, FunctionPriority priority)
    {
        if (term->type() != TermType::TERM_FUNCTION) {
            return FunctionTypeInfo[FunctionType::FUNCTION_USER_DEFINED].priority() < priority;
        } else {
            return FunctionTypeInfo[((Function *)term)->op()].priority() < priority;
        }
    }
    
    void Function::info(std::ostream &os) const
    {
        switch (_op) {
            case FunctionType::FUNCTION_USER_DEFINED:
                os << _name;
                _arguments->info(os);
                break;
                
            case FunctionType::FUNCTION_NEGATIVE:
                ASSERT(_arguments->size() == 1);
                os << FunctionTypeInfo[_op].symbol() << term_space;
                if (term_has_higher_function_priority(_arguments->at(0), FunctionTypeInfo[_op].priority()) == false) {
                    os << term_function_mark_left;
                    _arguments->at(0)->info(os);
                    os << term_function_mark_right;
                } else {
                    _arguments->at(0)->info(os);
                }
                break;
                
            case FunctionType::FUNCTION_PLUS:
            case FunctionType::FUNCTION_MULTIPLY:
                ASSERT(_arguments->size() == 2);
                if (term_has_lower_function_priority(_arguments->at(0), FunctionTypeInfo[_op].priority())) {
                    os << term_function_mark_left;
                    _arguments->at(0)->info(os);
                    os << term_function_mark_right;
                } else {
                    _arguments->at(0)->info(os);
                }
                os << term_space << FunctionTypeInfo[_op].symbol() << term_space;
                
                _arguments->at(1)->info(os);
                break;
                
            case FunctionType::FUNCTION_MINUS:
            case FunctionType::FUNCTION_DIVIDE:
                ASSERT(_arguments->size() == 2);
                _arguments->at(0)->info(os);
                os << term_space << FunctionTypeInfo[_op].symbol() << term_space;
                if (term_has_higher_function_priority(_arguments->at(1), FunctionTypeInfo[_op].priority()) == false) {
                    os << term_function_mark_left;
                    _arguments->at(1)->info(os);
                    os << term_function_mark_right;
                } else {
                    _arguments->at(1)->info(os);
                }
                break;
                
            default:
                ASSERT1(false, "function type not supported");
                break;
        }
    }
    
    Function * make_function(const std::string &name, Tuple *arguments, emc::Namer &namer)
    {
        return new Function(namer.constant(name), arguments);
    }
    
    Function * make_negative(Term *term)
    {
        return new Function(FunctionType::FUNCTION_NEGATIVE, make_tuple({term}));
    }
    
    Function * make_plus(Term *left, Term *right)
    {
        return new Function(FunctionType::FUNCTION_PLUS, make_tuple({left, right}));
    }
    
    Function * make_minus(Term *left, Term *right)
    {
        return new Function(FunctionType::FUNCTION_MINUS, make_tuple({left, right}));
    }
    
    Function * make_multiply(Term *left, Term *right)
    {
        return new Function(FunctionType::FUNCTION_MULTIPLY, make_tuple({left, right}));
    }
    
    Function * make_divide(Term *left, Term *right)
    {
        return new Function(FunctionType::FUNCTION_DIVIDE, make_tuple({left, right}));
    }
}