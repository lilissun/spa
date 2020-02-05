//
//  name.cpp
//  symbolic
//
//  Created by Li Li on 26/5/15.
//  Copyright (c) 2015 Lilissun. All rights reserved.
//

#include "name.h"

#include "common/debug.h"
#include "unifier.h"
#include "explicit/renamer.h"
#include "explicit/value.h"

namespace sym {
    bool NamePattern::match(const Term *term, const Unifier &unifier) const
    {
        if (term->type() == TermType::TERM_NAME) {
            const Name *name = (const Name *)term;
            if (name->name() == _name) {
                return true;
            } else {
                return false;
            }
        } else {
            return false;
        }
    }
    
    void NamePattern::info(std::ostream &os) const
    {
        os << _name << term_name_mark;
    }
    
    void Name::unify(const Term *term, Unifier &unifier) const
    {
        if (term->type() == TermType::TERM_VARIABLE) {
            return term->unify(this, unifier);
        } else if (term->type() == TermType::TERM_NAME) {
            auto name = (const Name *)term;
            if (name->name() != _name) {
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
    
    void Name::unify_to(const Term *term, Unifier &unifier) const
    {
        if (term->type() == TermType::TERM_NAME) {
            auto name = (const Name *)term;
            if (name->name() != _name) {
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
    
    bool Name::match(const Pattern *pattern, const Unifier &unifier) const
    {
        return pattern == nullptr || pattern->match(this, unifier);
    }
    
    Term * Name::clone() const
    {
        return new Name(_name);
    }
    
    Term * Name::map(const Unifier &unifier) const
    {
        return new Name(_name);
    }
    
    Term * Name::rename(emc::Renamer &renamer, emc::Namer &namer)
    {
        return this;
    }
    
    size_t Name::depth(const emc::Name &name, const Unifier &unifier) const
    {
        if (name == _name) {
            return 1;
        } else {
            return 0;
        }
    }
    
    size_t Name::weight() const
    {
        return TermType::TERM_NAME;
    }
    
    void Name::subnames(unsigned long mask, const Unifier &unifier, std::set<emc::Name> &names) const
    {
        subnames(mask, names);
    }
    
    void Name::subnames(unsigned long mask, std::set<emc::Name> &names) const
    {
        if (mask & TermType::TERM_NAME) {
            names.insert(_name);
        }
        return ;
    }
    
    void Name::signature(const Unifier &unifier, std::ostream &os) const
    {
        if (unifier.exist(_name)) {
            unifier.evaluate(_name)->signature(unifier, os);
        } else {
            signature(os);
        }
    }
    
    void Name::signature(std::ostream &os) const
    {
        os << _name.id();
    }
    
    emc::Value Name::evaluate(const emc::Valuator &valuator) const
    {
        NOT_IMPL;
    }
    
    void Name::info(std::ostream &os) const
    {
        os << _name << term_name_mark;
    }
    
    Name * make_name(const std::string &name, emc::Namer &namer)
    {
        return new Name(namer.constant(name));
    }
}