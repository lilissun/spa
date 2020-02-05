//
//  name.h
//  symbolic
//
//  Created by Li Li on 26/5/15.
//  Copyright (c) 2015 Lilissun. All rights reserved.
//

#ifndef __symbolic__name__
#define __symbolic__name__

#include <iostream>

#include "explicit/name.h"
#include "term.h"

namespace sym {
    class NamePattern : public Pattern {
    protected:
        emc::Name _name;
    
    public:
        NamePattern(emc::Name name) : Pattern(TermType::TERM_NAME), _name(name) {}
        virtual ~NamePattern() {}
        
    public:
        virtual bool match(const Term *term, const Unifier &unifier) const;
        
    public:
        virtual void info(std::ostream &os) const;
    };
    
    class Name : public Term {
    protected:
        emc::Name _name;
        
    public:
        Name(const emc::Name &name) : Term(TermType::TERM_NAME), _name(name) {}
        virtual ~Name() {}
        
    public:
        virtual bool same(const emc::Name &name) const { return _name == name; }
        virtual const emc::Name & name() const { return _name; }
        
    protected:
        virtual TermSort _sort() const { return TermSort::TERM_SORT_USER_DEFINED; }
        
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
    
    Name * make_name(const std::string &name, emc::Namer &namer);
}

#endif /* defined(__symbolic__name__) */
