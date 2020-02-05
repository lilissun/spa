//
//  constant.cpp
//  symbolic
//
//  Created by Li Li on 8/5/15.
//  Copyright (c) 2015 Lilissun. All rights reserved.
//

#include "constant.h"

#include "common/debug.h"
#include "common/runtime.h"
#include "explicit/namer.h"
#include "explicit/renamer.h"
#include "explicit/valuator.h"
#include "unifier.h"

namespace sym {
    bool ConstantPattern::match(const Term *term, const Unifier &unifier) const
    {
        return term->type() == TermType::TERM_CONSTANT;
    }
    
    void ConstantPattern::info(std::ostream &os) const
    {
        os << term_constant_mark;
    }
    
    void Constant::unify(const Term *term, Unifier &unifier) const
    {
        NOT_IMPL;
    }
    
    void Constant::unify_to(const Term *term, Unifier &unifier) const
    {
        NOT_IMPL;
    }
    
    bool Constant::match(const Pattern *pattern, const Unifier &unifier) const
    {
        return pattern == nullptr || pattern->match(this, unifier);
    }
    
    Term * Constant::clone() const
    {
        return new Constant(_name);
    }
    
    Term * Constant::map(const Unifier &unifier) const
    {
        return clone();
    }
    
    Term * Constant::rename(emc::Renamer &renamer, emc::Namer &namer)
    {
        _name = renamer.rename(_name, namer);
        return this;
    }
    
    size_t Constant::depth(const emc::Name &name, const Unifier &unifier) const
    {
        if (_name == name) {
            return 1;
        } else {
            return 0;
        }
    }
    
    size_t Constant::weight() const
    {
        return TermType::TERM_CONSTANT;
    }
    
    void Constant::subnames(unsigned long mask, const Unifier &unifier, std::set<emc::Name> &names) const
    {
        subnames(mask, names);
    }
    
    void Constant::subnames(unsigned long mask, std::set<emc::Name> &names) const
    {
        if (mask & TermType::TERM_CONSTANT) {
            names.insert(_name);
        }
    }
    
    void Constant::signature(const Unifier &unifier, std::ostream &os) const
    {
        signature(os);
    }
    
    void Constant::signature(std::ostream &os) const {
        os << _name.id();
    }
    
    emc::Value Constant::evaluate(const emc::Valuator &valuator) const
    {
        return valuator.evaluate(_name);
    }
    
    void Constant::info(std::ostream &os) const
    {
        os << term_constant_mark << _name;
    }
    
    Constant * make_constant(const std::string &name, emc::Namer &namer)
    {
        return new Constant(namer.constant(name));
    }
    
    Constant * make_constant(const emc::Name &name)
    {
        return new Constant(name);
    }
}