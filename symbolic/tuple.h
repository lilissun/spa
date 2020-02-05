//
//  tuple.h
//  symbolic
//
//  Created by Li Li on 22/5/15.
//  Copyright (c) 2015 Lilissun. All rights reserved.
//

#ifndef __symbolic__tuple__
#define __symbolic__tuple__

#include <iostream>
#include <vector>

#include "term.h"

namespace sym {
    class TuplePattern : public Pattern {
        
        friend TuplePattern * make_tuple_pattern(const std::vector<Pattern *> &arguments);
        
    protected:
        std::vector<Pattern *> _arguments; // refered
        
    protected:
        TuplePattern() : Pattern(TermType::TERM_TUPLE) {}
        TuplePattern(const std::vector<Pattern *> &arguments) : Pattern(TermType::TERM_TUPLE), _arguments(arguments) {}

    public:
        virtual ~TuplePattern() {}
        
    public:
        const Pattern * at(size_t index) const { return _arguments.at(index); }
        TuplePattern * append(Pattern *pattern) { _arguments.push_back(pattern); return this; }
        size_t size() const { return _arguments.size(); }
        
    public:
        virtual bool match(const Term *term, const Unifier &unifier) const;
        
    public:
        virtual void info(std::ostream &os) const;
    };
    
    TuplePattern * make_tuple_pattern(const std::vector<Pattern *> &arguments = {});

    // tuple is a special function with no name.
    // only the tuples with the same length are considerred as unifiable. 
    class Tuple : public Term {
        friend Tuple * make_tuple(const std::vector<Term *> &arguments);
        
    protected:
        std::vector<Term *> _arguments; // owned
        
    protected:
        Tuple() : Term(TermType::TERM_TUPLE) {}
        Tuple(const std::vector<Term *> arguments) : Term(TermType::TERM_TUPLE), _arguments(arguments) {}
        
    public:
        virtual ~Tuple();
        
    public:
        virtual bool same(const emc::Name &name) const { return false; }
        virtual const emc::Name & name() const;
        
    public:
        const Term * at(size_t index) const { return _arguments.at(index); }
        Tuple * append(Term *term) { _arguments.push_back(term); return this; }
        size_t size() const { return _arguments.size(); }
        
    public:
        void tuple_signature(size_t size, const Unifier &unifier, std::ostream &os) const;
        void tuple_signature(size_t size, std::ostream &os) const;
        bool tuple_can_unify(size_t size, const Tuple *tuple, Unifier &unifier) const;
        bool tuple_can_unify_to(size_t size, const Tuple *tuple, Unifier &unifier) const;
        
    public:
        Tuple * tuple_clone(size_t begin, size_t end) const;
        Tuple * tuple_clone(size_t size) const;
        Tuple * tuple_clone() const;
        Tuple * tuple_map(size_t begin, size_t end, const Unifier &unifier) const;
        Tuple * tuple_map(size_t size, const Unifier &unifier) const;
        Tuple * tuple_map(const Unifier &unifier) const;
        
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

    Tuple * make_tuple(const std::vector<Term *> &arguments = {});
}

#endif /* defined(__symbolic__tuple__) */
