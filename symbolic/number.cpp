//
//  number.cpp
//  symbolic
//
//  Created by Li Li on 18/1/15.
//  Copyright (c) 2015 Lilissun. All rights reserved.
//

#include "number.h"

#include "common/debug.h"
#include "common/runtime.h"
#include "unifier.h"
#include "explicit/namer.h"
#include "explicit/indexer.h"
#include "explicit/renamer.h"
#include "explicit/valuator.h"

namespace sym {
    bool NumberPattern::match(const Term *term, const Unifier &unifier) const
    {
        return term->type() == TermType::TERM_NUMBER;
    }
    
    void NumberPattern::info(std::ostream &os) const
    {
        os << term_number_mark;
    }
    
    void Number::unify(const Term *term, Unifier &unifier) const
    {
        if (term->type() == TermType::TERM_NUMBER) {
            auto number = (const Number *)term;
            // This trick is used to prevent the bi-direction loop between two numbers
            auto this_mapped_term = unifier.evaluate(_name);
            if (this_mapped_term != nullptr) {
                this_mapped_term->unify(number, unifier);
            } else if (_name != number->_name) {
                auto mapped_term = unifier.map(number->name(), this);
                if (mapped_term != nullptr) {
                    mapped_term->unify(this, unifier);
                }
            }
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
    
    void Number::unify_to(const Term *term, Unifier &unifier) const
    {
        if (term->type() == TermType::TERM_NUMBER) {
            auto mapped_term = unifier.map(_name, term);
            if (mapped_term != nullptr && mapped_term->equal(term) == false) {
                throw UnifyFailed();
            }
        } else if (term->type() == TermType::TERM_CONSTANT) {
            NOT_IMPL;
        } else if (term->type() == TermType::TERM_VALUE) {
            NOT_IMPL;
        } else {
            throw UnifyFailed();
        }
    }
    
    bool Number::match(const Pattern *pattern, const Unifier &unifier) const
    {
        return pattern == nullptr || pattern->match(this, unifier);
    }
    
    Term * Number::clone() const
    {
        return new Number(_name);
    }
    
    Term * Number::map(const Unifier &unifier) const
    {
        if (unifier.exist(_name) == true) {
            return unifier.evaluate(_name)->map(unifier);
        } else {
            return clone();
        }
    }
    
    Term * Number::rename(emc::Renamer &renamer, emc::Namer &namer)
    {
        _name = renamer.rename(_name, namer);
        return this;
    }
    
    size_t Number::depth(const emc::Name &name, const Unifier &unifier) const
    {
        auto term = unifier.evaluate(_name);
        if (term == nullptr) {
            if (_name == name) {
                return 1;
            } else {
                return 0;
            }
        } else {
            return term->depth(name, unifier);
        }
    }
    
    size_t Number::weight() const
    {
        return TermType::TERM_NUMBER;
    }
    
    void Number::subnames(unsigned long mask, const Unifier &unifier, std::set<emc::Name> &names) const
    {
        auto term = unifier.evaluate(_name);
        if (term != nullptr) {
            term->subnames(mask, unifier, names);
        } else {
            subnames(mask, names);
        }
    }
    
    void Number::subnames(unsigned long mask, std::set<emc::Name> &names) const
    {
        if (mask & TermType::TERM_NUMBER) {
            names.insert(_name);
        }
    }
    
    void Number::signature(const Unifier &unifier, std::ostream &os) const
    {
        if (unifier.exist(_name)) {
            unifier.evaluate(_name)->signature(unifier, os);
        } else {
            signature(os);
        }
    }
    
    void Number::signature(std::ostream &os) const {
        os << _name.id();
    }
    
    emc::Value Number::evaluate(const emc::Valuator &valuator) const
    {
        return valuator.evaluate(_name);
    }
    
    void Number::info(std::ostream &os) const
    {
        os << term_number_mark << _name;
    }
    
    Number * make_number(const std::string &name, emc::Namer &namer, emc::Indexer &indexer)
    {
        return new Number(indexer.variable(name, namer));
    }
    
    Number * make_number(const std::string &name, emc::Namer &namer)
    {
        return new Number(namer.variable(name));
    }
    
    Number * make_number(const std::string &name, emc::Name::ID id, emc::Namer &namer)
    {
        return new Number(namer.variable(name, id));
    }
    
    Number * make_number(const emc::Name &name)
    {
        return new Number(name);
    }
}