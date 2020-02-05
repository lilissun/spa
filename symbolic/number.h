//
//  number.h
//  symbolic
//
//  Created by Li Li on 18/1/15.
//  Copyright (c) 2015 Lilissun. All rights reserved.
//

#ifndef __symbolic__number__
#define __symbolic__number__

#include <iostream>

#include "explicit/name.h"
#include "term.h"

namespace sym {
    class NumberPattern : public Pattern {
    public:
        NumberPattern() : Pattern(TermType::TERM_NUMBER) {}
        virtual ~NumberPattern() {}
        
    public:
        virtual bool match(const Term *term, const Unifier &unifier) const;
        
    public:
        virtual void info(std::ostream &os) const;
    };
    
    class Number : public Term {
        
        friend Number * make_number(const std::string &name, emc::Namer &namer, emc::Indexer &indexer);
        friend Number * make_number(const std::string &name, emc::Namer &namer);
        friend Number * make_number(const std::string &name, emc::Name::ID id, emc::Namer &namer);
        friend Number * make_number(const emc::Name &name);
        
    protected:
        emc::Name _name;

    protected:
        Number(const emc::Name &name) : Term(TermType::TERM_NUMBER), _name(name) {}
        
    public:
        virtual ~Number() {}
        
    public:
        virtual bool same(const emc::Name &name) const { return _name == name; }
        virtual const emc::Name & name() const { return _name; }
        
    protected:
        virtual TermSort _sort() const { return TermSort::TERM_SORT_NUMBER; }
        
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
    
    Number * make_number(const std::string &name, emc::Namer &namer, emc::Indexer &indexer);
    Number * make_number(const std::string &name, emc::Namer &namer);
    Number * make_number(const std::string &name, emc::Name::ID id, emc::Namer &namer);
    Number * make_number(const emc::Name &name);
}

#endif /* defined(__symbolic__number__) */
