//
//  constant.h
//  symbolic
//
//  Created by Li Li on 8/5/15.
//  Copyright (c) 2015 Lilissun. All rights reserved.
//

#ifndef __symbolic__constant__
#define __symbolic__constant__

#include <iostream>

#include "explicit/name.h"
#include "term.h"

namespace sym {
    class ConstantPattern : public Pattern {
    public:
        ConstantPattern() : Pattern(TermType::TERM_CONSTANT) {}
        virtual ~ConstantPattern() {}
        
    public:
        virtual bool match(const Term *term, const Unifier &unifier) const;
        
    public:
        virtual void info(std::ostream &os) const;
    };
    
    class Constant : public Term {
        
        friend Constant * make_constant(const std::string &name, emc::Namer &namer);
        friend Constant * make_constant(const emc::Name &name);
        
    protected:
        emc::Name _name;
        
    protected:
        Constant(const emc::Name &name) : Term(TermType::TERM_CONSTANT), _name(name) {}
        
    public:
        virtual ~Constant() {}
        
    public:
        virtual bool same(const emc::Name &name) const { return _name == name; }
        virtual const emc::Name & name() const { return _name; }
        
    protected:
        virtual TermSort _sort() const { return TermSort::TERM_SORT_LINEAR_EXPRESSION; }
        
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
    
    Constant * make_constant(const std::string &name, emc::Namer &namer);
    Constant * make_constant(const emc::Name &name);
}

#endif /* defined(__symbolic__constant__) */
