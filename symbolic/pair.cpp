//
//  pair.cpp
//  symbolic
//
//  Created by Li Li on 20/8/15.
//  Copyright (c) 2015 Lilissun. All rights reserved.
//

#include "pair.h"

#include "common/debug.h"
#include "common/runtime.h"
#include "explicit/renamer.h"
#include "explicit/value.h"
#include "unifier.h"
#include "matcher.h"

namespace sym {
    bool PairPattern::match(const Term *term, const Unifier &unifier) const
    {
        if (term->type() != TermType::TERM_PAIR) {
            return false;
        }
        
        auto pair = (const Pair *)term;
        if (pair->first()->match(_first, unifier) == false) {
            return false;
        } else if (pair->second()->match(_second, unifier) == false) {
            return false;
        } else {
            return true;
        }
    }
    
    void PairPattern::info(std::ostream &os) const
    {
        _first->info(os);
        os << term_argument_separator << term_space;
        _second->info(os);
    }
    
    PairPattern * make_pair_pattern(Pattern *first, Pattern *second)
    {
        return new PairPattern(first, second);
    }
    
    Pair::~Pair()
    {
        delete _first;
        delete _second;
    }
    
    const emc::Name & Pair::name() const
    {
        ASSERT(false);
        return emc::INVALID_NAME;
    }

    void Pair::unify(const Term *term, Unifier &unifier) const
    {
        if (term->type() == TermType::TERM_VARIABLE) {
            return term->unify(this, unifier);
        } else if (term->type() == TermType::TERM_PAIR) {
            const Pair *pair = (const Pair *)term;
            _first->unify(pair->_first, unifier);
            _second->unify(pair->_second, unifier);
        } else if (term->type() == TermType::TERM_CONSTANT) {
            NOT_IMPL;
        } else if (term->type() == TermType::TERM_VALUE) {
            NOT_IMPL;
        } else {
            throw UnifyFailed();
        }
    }
    
    void Pair::unify_to(const Term *term, Unifier &unifier) const
    {
        if (term->type() == TermType::TERM_PAIR) {
            const Pair *pair = (const Pair *)term;
            _first->unify_to(pair->_first, unifier);
            _second->unify_to(pair->_second, unifier);
        } else if (term->type() == TermType::TERM_CONSTANT) {
            NOT_IMPL;
        } else if (term->type() == TermType::TERM_VALUE) {
            NOT_IMPL;
        } else {
            throw UnifyFailed();
        }
    }
    
    bool Pair::match(const Pattern *pattern, const Unifier &unifier) const
    {
        return pattern == nullptr || pattern->match(this, unifier);
    }
    
    TermSort Pair::_sort() const
    {
        // make sure the arguments are user-defined
        if (_first->sort() != TermSort::TERM_SORT_USER_DEFINED) {
            return TermSort::TERM_SORT_COMPLEX;
        } else if (_second->sort() != TermSort::TERM_SORT_USER_DEFINED) {
            return TermSort::TERM_SORT_COMPLEX;
        } else {
            return TermSort::TERM_SORT_USER_DEFINED;
        }
    }
    
    
    Term * Pair::clone() const
    {
        return new Pair(_first->clone(), _second->clone());
    }
    
    Term * Pair::map(const Unifier &unifier) const
    {
        return new Pair(_first->map(unifier), _second->map(unifier));
    }
    
    Term * Pair::rename(emc::Renamer &renamer, emc::Namer &namer)
    {
        _first->rename(renamer, namer);
        _second->rename(renamer, namer);
        return this;
    }
    
    size_t Pair::depth(const emc::Name &name, const Unifier &unifier) const
    {
        auto depth_first = _first->depth(name, unifier);
        auto depth_second = _second->depth(name, unifier);
        auto depth = depth_first > depth_second ? depth_first : depth_second;
        return depth == 0 ? 0 : depth + 1;
    }
    
    size_t Pair::weight() const
    {
        return _first->weight() + _second->weight();
    }
    
    void Pair::subnames(unsigned long mask, const Unifier &unifier, std::set<emc::Name> &names) const
    {
        _first->subnames(mask, unifier, names);
        _second->subnames(mask, unifier, names);
    }
    
    void Pair::subnames(unsigned long mask, std::set<emc::Name> &names) const
    {
        _first->subnames(mask, names);
        _second->subnames(mask, names);
    }
    
    void Pair::signature(const Unifier &unifier, std::ostream &os) const
    {
        os << term_pair_mark_left;
        _first->signature(unifier, os);
        os << term_signature_separator;
        _second->signature(unifier, os);
        os << term_pair_mark_right;
    }
    
    void Pair::signature(std::ostream &os) const
    {
        os << term_pair_mark_left;
        _first->signature(os);
        os << term_signature_separator;
        _second->signature(os);
        os << term_pair_mark_right;
    }
    
    emc::Value Pair::evaluate(const emc::Valuator &valuator) const
    {
        NOT_IMPL;
    }
    
    void Pair::info(std::ostream &os) const
    {
        os << term_pair_mark_left;
        _first->info(os);
        os << term_argument_separator << term_space;
        _second->info(os);
        os << term_pair_mark_right;
    }
    
    Pair * make_pair(Term *first, Term *second) {
        return new Pair(first, second);
    }
}