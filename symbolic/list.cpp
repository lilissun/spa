//
//  list.cpp
//  symbolic
//
//  Created by Li Li on 20/8/15.
//  Copyright (c) 2015 Lilissun. All rights reserved.
//

#include "list.h"

#include "common/debug.h"
#include "common/runtime.h"
#include "explicit/renamer.h"
#include "explicit/value.h"
#include "unifier.h"
#include "matcher.h"
#include "pair.h"
#include "variable.h"

namespace sym {
    bool ListPattern::match(const Term *term, const Unifier &unifier) const
    {
        if (term->type() != TermType::TERM_LIST) {
            return false;
        }
        
        return term->match(_pattern, unifier);
    }
    
    void ListPattern::info(std::ostream &os) const
    {
        NOT_IMPL;
    }
    
    ListPattern * make_list_pattern(Pattern *pattern)
    {
        return new ListPattern(pattern);
    }
    
    List::~List()
    {
        delete _list;
    }
    
    const emc::Name & List::name() const
    {
        ASSERT(false);
        return emc::INVALID_NAME;
    }
    
    void List::unify(const Term *term, Unifier &unifier) const
    {
        if (term->type() == TermType::TERM_VARIABLE) {
            return term->unify(this, unifier);
        } else if (term->type() == TermType::TERM_LIST) {
            const List *list = (const List *)term;
            _list->unify(list->_list, unifier);
        } else if (term->type() == TermType::TERM_CONSTANT) {
            NOT_IMPL;
        } else if (term->type() == TermType::TERM_VALUE) {
            NOT_IMPL;
        } else {
            throw UnifyFailed();
        }
    }
    
    void List::unify_to(const Term *term, Unifier &unifier) const
    {
        if (term->type() == TermType::TERM_LIST) {
            const List *list = (const List *)term;
            _list->unify_to(list->_list, unifier);
        } else if (term->type() == TermType::TERM_CONSTANT) {
            NOT_IMPL;
        } else if (term->type() == TermType::TERM_VALUE) {
            NOT_IMPL;
        } else {
            throw UnifyFailed();
        }
    }
    
    bool List::match(const Pattern *pattern, const Unifier &unifier) const
    {
        return pattern == nullptr || pattern->match(this, unifier);
    }
    
    TermSort List::_sort() const
    {
        // make sure the arguments are user-defined
        if (_list->sort() == TermSort::TERM_SORT_USER_DEFINED) {
            return TermSort::TERM_SORT_USER_DEFINED;
        } else {
            return TermSort::TERM_SORT_COMPLEX;
        }
    }
    
    
    Term * List::clone() const
    {
        return new List(_list->clone());
    }
    
    Term * List::map(const Unifier &unifier) const
    {
        return new List(_list->map(unifier));
    }
    
    Term * List::rename(emc::Renamer &renamer, emc::Namer &namer)
    {
        _list->rename(renamer, namer);
        return this;
    }
    
    size_t List::depth(const emc::Name &name, const Unifier &unifier) const
    {
        size_t depth = 0;
        const Term *term = _list;
        while (term->type() == TermType::TERM_PAIR) {
            auto pair = (const Pair *)term;
            auto current_depth = pair->first()->depth(name, unifier);
            depth = depth > current_depth ? depth : current_depth;
            term = pair->second();
        }
        if (term->type() != TermType::TERM_VARIABLE) {
            auto current_depth = term->depth(name, unifier);
            depth = depth > current_depth ? depth : current_depth;
        }
        return depth == 0 ? 0 : depth + 1;
    }
    
    size_t List::weight() const
    {
        size_t weight = 0;
        const Term *term = _list;
        while (term->type() == TermType::TERM_PAIR) {
            auto pair = (const Pair *)term;
            weight += pair->first()->weight();
            term = pair->second();
        }
        if (term->type() != TermType::TERM_VARIABLE) {
            weight += term->weight();
        }
        return weight;
    }
    
    void List::subnames(unsigned long mask, const Unifier &unifier, std::set<emc::Name> &names) const
    {
        const Term *term = _list;
        while (term->type() == TermType::TERM_PAIR) {
            auto pair = (const Pair *)term;
            pair->first()->subnames(mask, unifier, names);
            term = pair->second();
        }
        if (term->type() != TermType::TERM_VARIABLE) {
            term->subnames(mask, unifier, names);
        }

    }
    
    void List::subnames(unsigned long mask, std::set<emc::Name> &names) const
    {
        const Term *term = _list;
        while (term->type() == TermType::TERM_PAIR) {
            auto pair = (const Pair *)term;
            pair->first()->subnames(mask, names);
            term = pair->second();
        }
        if (term->type() != TermType::TERM_VARIABLE) {
            term->subnames(mask, names);
        }
    }
    
    void List::signature(const Unifier &unifier, std::ostream &os) const
    {
        os << term_list_mark_left;
        const Term *term = _list;
        while (term->type() == TermType::TERM_PAIR) {
            auto pair = (const Pair *)term;
            pair->first()->signature(unifier, os);
            term = pair->second();
            os << term_signature_separator;
        }
        if (term->type() != TermType::TERM_VARIABLE) {
            term->info(os);
        }
        os << term_list_mark_right;
    }
    
    void List::signature(std::ostream &os) const
    {
        os << term_list_mark_left;
        const Term *term = _list;
        while (term->type() == TermType::TERM_PAIR) {
            auto pair = (const Pair *)term;
            pair->first()->signature(os);
            term = pair->second();
            os << term_signature_separator;
        }
        if (term->type() != TermType::TERM_VARIABLE) {
            term->info(os);
        }
        os << term_list_mark_right;
    }
    
    emc::Value List::evaluate(const emc::Valuator &valuator) const
    {
        NOT_IMPL;
    }
    
    void List::info(std::ostream &os) const
    {
        os << term_list_mark_left;
        const Term *term = _list;
        while (term->type() == TermType::TERM_PAIR) {
            if (term != _list) {
                os << term_argument_separator << term_space;
            }
            auto pair = (const Pair *)term;
            pair->first()->info(os);
            term = pair->second();
        }
        if (term->type() != TermType::TERM_VARIABLE) {
            os << term_argument_separator << term_space;
            term->info(os);
        }
        os << term_list_mark_right;
    }
    
    List * make_list(const std::vector<Term *> &arguments, sym::Term *tail)
    {
        Term *list = tail;
        for (auto i = arguments.rbegin(); i != arguments.rend(); ++i) {
            list = make_pair(*i, list);
        }
        return new List(list);
    }
    
    List * make_list(sym::Term *list)
    {
        return new List(list);
    }
}