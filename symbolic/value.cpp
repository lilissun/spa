//
//  value.cpp
//  symbolic
//
//  Created by Li Li on 18/1/15.
//  Copyright (c) 2015 Lilissun. All rights reserved.
//

#include "value.h"

#include "unifier.h"
#include "common/debug.h"
#include "explicit/renamer.h"
#include "explicit/value.h"

namespace sym {
    bool ValuePattern::match(const Term *term, const Unifier &unifier) const
    {
        return term->type() == TermType::TERM_VALUE;
    }
    
    void ValuePattern::info(std::ostream &os) const
    {
        os << term_value_mark;
    }

    Term * Value::clone() const {
        return new Value(_value);
    }
    
    Term * Value::map(const Unifier &unifier) const
    {
        return clone();
    }
    
    Term * Value::rename(emc::Renamer &renamer, emc::Namer &namer)
    {
        return this;
    }
    
    void Value::unify(const Term *term, Unifier &unifier) const
    {
        NOT_IMPL;
    }
    
    void Value::unify_to(const Term *term, Unifier &unifier) const
    {
        NOT_IMPL;
    }
    
    bool Value::match(const Pattern *pattern, const Unifier &unifier) const
    {
        return pattern == nullptr || pattern->match(this, unifier);
    }
    
    size_t Value::depth(const emc::Name &name, const Unifier &unifier) const
    {
        return 0;
    }
    
    size_t Value::weight() const
    {
        return TermType::TERM_VALUE;
    }
    
    void Value::subnames(unsigned long mask, const Unifier &unifier, std::set<emc::Name> &names) const
    {
        subnames(mask, names);
    }
    
    void Value::subnames(unsigned long mask, std::set<emc::Name> &names) const
    {
        return ;
    }
    
    void Value::signature(const Unifier &unifier, std::ostream &os) const
    {
        signature(os);
    }
    
    void Value::signature(std::ostream &os) const
    {
        os << _value;
    }

    emc::Value Value::evaluate(const emc::Valuator &valuator) const
    {
        return _value;
    }
    
    void Value::info(std::ostream &os) const
    {
        os << _value;
    }
    
    Value * make_value(long value)
    {
        return new Value(emc::Value(value));
    }
}