//
//  variable.h
//  symbolic
//
//  Created by Li Li on 24/8/13.
//  Copyright (c) 2013 Lilissun. All rights reserved.
//

#ifndef __symbolic__variable__
#define __symbolic__variable__

#include <iostream>
#include <vector>

#include "explicit/name.h"
#include "term.h"

namespace sym {
    class VariablePattern : public Pattern {
    protected:
        std::vector<const Pattern *> _patterns; // referred
        
    public:
        VariablePattern() : Pattern(TermType::TERM_VARIABLE) {}
        virtual ~VariablePattern();

    public:
        VariablePattern * append(Pattern *pattern) { if (pattern != this) { _patterns.push_back(pattern); } return this; }
        
    public:
        virtual bool match(const Term *term, const Unifier &unifier) const;
        
    public:
        virtual void info(std::ostream &os) const;
    };

    class Variable : public Term {
        
        friend Variable * make_variable(const std::string &name, emc::Namer &namer, emc::Indexer &indexer);
        friend Variable * make_variable(const std::string &name, emc::Namer &namer);
        friend Variable * make_variable(const std::string &name, emc::Name::ID id, emc::Namer &namer);
        
    protected:
        emc::Name _name;
        
    protected:
        Variable(const emc::Name &name) : Term(TermType::TERM_VARIABLE), _name(name) {}
        
    public:
        virtual ~Variable() {}
        
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
    
    Variable * make_variable(const std::string &name, emc::Namer &namer, emc::Indexer &indexer);
    Variable * make_variable(const std::string &name, emc::Namer &namer);
    Variable * make_variable(const std::string &name, emc::Name::ID id, emc::Namer &namer);
}

#endif /* defined(__symbolic__variable__) */
