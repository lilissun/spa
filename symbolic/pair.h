//
//  pair.h
//  symbolic
//
//  Created by Li Li on 20/8/15.
//  Copyright (c) 2015 Lilissun. All rights reserved.
//

#ifndef __symbolic__pair__
#define __symbolic__pair__

#include <iostream>
#include <vector>

#include "term.h"

namespace sym {
    class PairPattern : public Pattern {
        
        friend PairPattern * make_pair_pattern(Pattern *first, Pattern *second);
        
    protected:
        Pattern *_first; // refered
        Pattern *_second; // refered
        
    protected:
        PairPattern(Pattern *first, Pattern *second) : Pattern(TermType::TERM_PAIR), _first(first), _second(second) {}

    public:
        virtual ~PairPattern() {}
        
    public:
        const Pattern * first() const { return _first; }
        const Pattern * second() const { return _second; }
        
    public:
        virtual bool match(const Term *term, const Unifier &unifier) const;
        
    public:
        virtual void info(std::ostream &os) const;
    };
    
    PairPattern * make_pair_pattern(Pattern *first, Pattern *second);
    
    class Pair : public Term {
        friend Pair * make_pair(Term *first, Term *second);
        
    protected:
        Term *_first; // owned
        Term *_second; // owned
        
    protected:
        Pair(Term *first, Term *second) : Term(TermType::TERM_PAIR), _first(first), _second(second) {}
        
    public:
       virtual ~Pair();
        
    public:
        virtual bool same(const emc::Name &name) const { return false; }
        virtual const emc::Name & name() const;
        
    public:
        const Term * first() const { return _first; }
        const Term * second() const { return _second; }
        
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
    
    Pair * make_pair(Term *first, Term *second);
}

#endif /* defined(__symbolic__pair__) */
