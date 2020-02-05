//
//  tuple.cpp
//  symbolic
//
//  Created by Li Li on 22/5/15.
//  Copyright (c) 2015 Lilissun. All rights reserved.
//

#include "tuple.h"

#include "common/debug.h"
#include "common/runtime.h"
#include "unifier.h"
#include "matcher.h"
#include "explicit/renamer.h"
#include "explicit/value.h"

namespace sym {
    bool TuplePattern::match(const Term *term, const Unifier &unifier) const
    {
        if (term->type() != TermType::TERM_TUPLE) {
            return false;
        }
        
        auto tuple = (const Tuple *)term;
        if (_arguments.size() != tuple->size()) {
            return false;
        }

        for (size_t i = 0; i != _arguments.size(); ++i) {
            if (tuple->at(i)->match(_arguments.at(i), unifier) == false) {
                return false;
            }
        }
        return true;
    }
    
    void TuplePattern::info(std::ostream &os) const
    {
        os << term_tuple_mark_left;
        for (auto argument = _arguments.begin(); argument != _arguments.end(); ++argument) {
            if (argument != _arguments.begin()) {
                os << term_argument_separator << term_space;
            }
            // if call info for arguments could lead to infinite depth pattern
            if (*argument == nullptr) {
                os << term_any_mark;
            } else {
                os << term_key_mark;
            }
        }
        os << term_tuple_mark_right;
    }
    
    TuplePattern * make_tuple_pattern(const std::vector<Pattern *> &arguments)
    {
        return new TuplePattern(arguments);
    }

    Tuple::~Tuple()
    {
        for(auto i = _arguments.begin(); i != _arguments.end(); ++i) {
            delete (*i);
        }
    }
    
    const emc::Name & Tuple::name() const
    {
        ASSERT(false);
        return emc::INVALID_NAME;
    }

    void Tuple::unify(const Term *term, Unifier &unifier) const
    {
        if (term->type() == TermType::TERM_VARIABLE) {
            return term->unify(this, unifier);
        } else if (term->type() == TermType::TERM_TUPLE) {
            const Tuple *tuple = (const Tuple *)term;
            if (_arguments.size() != tuple->size()) {
                throw UnifyFailed();
            }
            
            auto i = _arguments.begin();
            auto j = tuple->_arguments.begin();
            while (i != _arguments.end()) {
                (*i)->unify(*j, unifier);
                ++i;
                ++j;
            }
        } else if (term->type() == TermType::TERM_CONSTANT) {
            NOT_IMPL;
        } else if (term->type() == TermType::TERM_VALUE) {
            NOT_IMPL;
        } else {
            throw UnifyFailed();
        }
    }
    
    void Tuple::unify_to(const Term *term, Unifier &unifier) const
    {
        if (term->type() == TermType::TERM_TUPLE) {
            const Tuple *tuple = (const Tuple *)term;
            if (_arguments.size() != tuple->size()) {
                throw UnifyFailed();
            }

            auto i = _arguments.begin();
            auto j = tuple->_arguments.begin();
            while (i != _arguments.end()) {
                (*i)->unify_to(*j, unifier);
                ++i;
                ++j;
            }
        } else if (term->type() == TermType::TERM_CONSTANT) {
            NOT_IMPL;
        } else if (term->type() == TermType::TERM_VALUE) {
            NOT_IMPL;
        } else {
            throw UnifyFailed();
        }
    }
    
    bool Tuple::match(const Pattern *pattern, const Unifier &unifier) const
    {
        return pattern == nullptr || pattern->match(this, unifier);
    }
    
    void Tuple::tuple_signature(size_t size, const Unifier &unifier, std::ostream &os) const
    {
        ASSERT(size <= _arguments.size());
        os << term_tuple_mark_left;
        for (size_t i = 0; i != size; ++i) {
            if (i != 0) {
                os << term_signature_separator;
            }
            _arguments.at(i)->signature(unifier, os);
        }
        os << term_tuple_mark_right;
    }
    
    void Tuple::tuple_signature(size_t size, std::ostream &os) const
    {
        ASSERT(size <= _arguments.size());
        os << term_tuple_mark_left;
        for (size_t i = 0; i != size; ++i) {
            if (i != 0) {
                os << term_signature_separator;
            }
            _arguments.at(i)->signature(os);
        }
        os << term_tuple_mark_right;
    }
    
    Tuple * Tuple::tuple_clone(size_t begin, size_t end) const
    {
        ASSERT(begin <= end);
        ASSERT(end <= _arguments.size());
        Tuple *tuple = new Tuple();
        for (size_t i = begin; i != end; ++i) {
            tuple->append(_arguments.at(i)->clone());
        }
        return tuple;
    }

    Tuple * Tuple::tuple_clone(size_t size) const
    {
        ASSERT(size <= _arguments.size());
        Tuple *tuple = new Tuple();
        for (size_t i = 0; i != size; ++i) {
            tuple->append(_arguments.at(i)->clone());
        }
        return tuple;
    }
    
    Tuple * Tuple::tuple_map(size_t begin, size_t end, const Unifier &unifier) const
    {
        ASSERT(begin <= end);
        ASSERT(end <= _arguments.size());
        Tuple *tuple = new Tuple();
        for (size_t i = begin; i != end; ++i) {
            tuple->append(_arguments.at(i)->map(unifier));
        }
        return tuple;
    }
    
    Tuple * Tuple::tuple_map(size_t size, const Unifier &unifier) const
    {
        ASSERT(size <= _arguments.size());
        Tuple *tuple = new Tuple();
        for (size_t i = 0; i != size; ++i) {
            tuple->append(_arguments.at(i)->map(unifier));
        }
        return tuple;
    }

    Tuple * Tuple::tuple_clone() const
    {
        return tuple_clone(_arguments.size());
    }
    
    Tuple * Tuple::tuple_map(const Unifier &unifier) const
    {
        return tuple_map(_arguments.size(), unifier);
    }
    
    bool Tuple::tuple_can_unify(size_t size, const Tuple *tuple, Unifier &unifier) const
    {
        ASSERT(size <= _arguments.size());
        ASSERT(size <= tuple->_arguments.size());
        try {
            auto history = std::set<emc::Name>();
            for (size_t i = 0; i != size; ++i) {
                _arguments.at(i)->unify(tuple->_arguments.at(i), unifier);
            }
            return true;
        } catch (const UnifyFailed &e) {
            return false;
        }
    }
    
    bool Tuple::tuple_can_unify_to(size_t size, const Tuple *tuple, Unifier &unifier) const
    {
        ASSERT(size <= _arguments.size());
        ASSERT(size <= tuple->_arguments.size());
        try {
            for (size_t i = 0; i != size; ++i) {
                _arguments.at(i)->unify_to(tuple->_arguments.at(i), unifier);
            }
            return true;
        } catch (const UnifyFailed &e) {
            return false;
        }
    }
    
    TermSort Tuple::_sort() const
    {
        // make sure the arguments are user-defined
        for (auto i = _arguments.begin(); i != _arguments.end(); ++i) {
            if ((*i)->sort() != TermSort::TERM_SORT_USER_DEFINED) {
                return TermSort::TERM_SORT_COMPLEX;
            }
        }
        return TERM_SORT_USER_DEFINED;
    }

    
    Term * Tuple::clone() const
    {
        return tuple_clone();
    }
    
    Term * Tuple::map(const Unifier &unifier) const
    {
        return tuple_map(unifier);
    }
    
    Term * Tuple::rename(emc::Renamer &renamer, emc::Namer &namer)
    {
        for (auto i = _arguments.begin(); i != _arguments.end(); ++i) {
            (*i)->rename(renamer, namer);
        }
        return this;
    }
    
    size_t Tuple::depth(const emc::Name &name, const Unifier &unifier) const
    {
        size_t max_argument_depth = 0;
        for (auto argument = _arguments.begin(); argument != _arguments.end(); ++argument) {
            size_t argument_depth = (*argument)->depth(name, unifier);
            max_argument_depth = argument_depth > max_argument_depth ? argument_depth : max_argument_depth;
        }
        return max_argument_depth == 0 ? 0 : max_argument_depth + 1;
    }
    
    size_t Tuple::weight() const
    {
        size_t weight = 0;
        for (auto argument = _arguments.begin(); argument != _arguments.end(); ++argument) {
            weight += (*argument)->weight();
        }
        return weight;
    }
    
    void Tuple::subnames(unsigned long mask, const Unifier &unifier, std::set<emc::Name> &names) const
    {
        for (auto i = _arguments.begin(); i != _arguments.end(); ++i) {
            (*i)->subnames(mask, unifier, names);
        }
    }
    
    void Tuple::subnames(unsigned long mask, std::set<emc::Name> &names) const
    {
        for (auto i = _arguments.begin(); i != _arguments.end(); ++i) {
            (*i)->subnames(mask, names);
        }
    }
    
    void Tuple::signature(const Unifier &unifier, std::ostream &os) const
    {
        os << term_tuple_mark_left;
        for (auto i = _arguments.begin(); i != _arguments.end(); ++i) {
            if (i != _arguments.begin()) {
                os << term_signature_separator;
            }
            (*i)->signature(unifier, os);
        }
        os << term_tuple_mark_right;
    }
    
    void Tuple::signature(std::ostream &os) const
    {
        os << term_tuple_mark_left;
        for (auto i = _arguments.begin(); i != _arguments.end(); ++i) {
            if (i != _arguments.begin()) {
                os << term_signature_separator;
            }
            (*i)->signature(os);
        }
        os << term_tuple_mark_right;
    }
    
    emc::Value Tuple::evaluate(const emc::Valuator &valuator) const
    {
        NOT_IMPL;
    }
    
    void Tuple::info(std::ostream &os) const
    {
        os << term_tuple_mark_left;
        for (auto i = _arguments.begin(); i != _arguments.end(); ++i) {
            if (i != _arguments.begin()) {
                os << term_argument_separator << term_space;
            }
            (*i)->info(os);
        }
        os << term_tuple_mark_right;
    }
    
    Tuple * make_tuple(const std::vector<Term *> &arguments)
    {
        return new Tuple(arguments);
    }
}