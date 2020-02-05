//
//  value.h
//  symbolic
//
//  Created by Li Li on 18/1/15.
//  Copyright (c) 2015 Lilissun. All rights reserved.
//

#ifndef __symbolic__value__
#define __symbolic__value__

#include "explicit/value.h"
#include "term.h"

namespace sym {
    class ValuePattern : public Pattern {
    public:
        ValuePattern() : Pattern(TermType::TERM_VALUE) {}
        virtual ~ValuePattern() {}
        
    public:
        virtual bool match(const Term *term, const Unifier &unifier) const;
        
    public:
        virtual void info(std::ostream &os) const;
    };

    class Value : public Term {
        
        friend Value * make_value(long value);
        
    protected:
        emc::Value _value;
        
    protected:
        Value(const emc::Value &value) : Term(TermType::TERM_VALUE), _value(value) {}
        
    public:
        virtual ~Value() {}
        
    public:
        virtual bool same(const emc::Name &name) const { return false; }
        virtual const emc::Name &name() const { ASSERT(false); }
        
    public:
        const emc::Value & value() const { return _value; }
        
    protected:
        virtual TermSort _sort() const { return TermSort::TERM_SORT_VALUE; }
        
    public:
        virtual Term * clone() const;
        virtual Term * map(const Unifier &unifier) const;
        virtual Term * rename(emc::Renamer &renamer, emc::Namer &namer);
        
    public:
        virtual void unify(const Term *term, Unifier &unifier) const;
        virtual void unify_to(const Term *term, Unifier &unifier) const;
        virtual bool match(const Pattern *pattern, const Unifier &unifier) const;
        virtual size_t depth(const emc::Name &name, const Unifier &unifier) const;
        virtual size_t weight() const;
        
    public:
        virtual void subnames(unsigned long mask, const Unifier &unifier, std::set<emc::Name> &names) const;
        virtual void subnames(unsigned long mask, std::set<emc::Name> &names) const;
        virtual void signature(const Unifier &unifier, std::ostream &os) const;
        virtual void signature(std::ostream &os) const;
        
    public:
        virtual emc::Value evaluate(const emc::Valuator &valuator) const;
        
    public:
        virtual void info(std::ostream &os) const;
    };
    
    Value * make_value(long value);
}

#endif /* defined(__symbolic__value__) */
