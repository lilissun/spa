//
//  list.h
//  symbolic
//
//  Created by Li Li on 20/8/15.
//  Copyright (c) 2015 Lilissun. All rights reserved.
//

#ifndef __symbolic__list__
#define __symbolic__list__

#include <iostream>
#include <vector>

#include "term.h"

namespace sym {
    class Variable;
    
    class ListPattern : public Pattern {
        
        friend ListPattern * make_list_pattern(Pattern *pattern);
        
    protected:
        Pattern *_pattern; // refered
        
    protected:
        ListPattern(Pattern *pattern) : Pattern(TermType::TERM_LIST), _pattern(pattern) {}
        
    public:
        virtual ~ListPattern() {}
        
    public:
        size_t size() const;
        const Pattern * at(size_t index) const;
        
    public:
        virtual bool match(const Term *term, const Unifier &unifier) const;
        
    public:
        virtual void info(std::ostream &os) const;
    };
    
    ListPattern * make_list_pattern(Pattern *pattern);
    
    class List : public Term {
        
        friend List * make_list(const std::vector<Term *> &arguments, sym::Term *tail);
        friend List * make_list(sym::Term *list);
        
    protected:
        Term *_list; // owned
        
    protected:
        List(Term *list = nullptr) : Term(TermType::TERM_LIST), _list(list) {}
        
    public:
        virtual ~List();
        
    public:
        virtual bool same(const emc::Name &name) const { return false; }
        virtual const emc::Name & name() const;
        
    public:
        size_t size() const;
        const Term * at() const;
        
    protected:
        virtual TermSort _sort() const;
        
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
    
    List * make_list(const std::vector<Term *> &arguments, sym::Term *tail);
    List * make_list(sym::Term *list);
}

#endif /* defined(__symbolic__list__) */
