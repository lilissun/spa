//
//  state.h
//  symbolic
//
//  Created by Li Li on 31/10/13.
//  Copyright (c) 2013 Lilissun. All rights reserved.
//

#ifndef __symbolic__state__
#define __symbolic__state__

#include <iostream>
#include <vector>
#include <set>

#include "fact.h"
#include "explicit/name.h"

namespace sym {
    class Tuple;
    class TuplePattern;
    class State : public Fact {
        
        friend State * make_state(const std::string &name, size_t keycount, Tuple *arguments, TuplePattern *patterns, emc::Namer &namer);
        
    protected:
        emc::Name _name;
        size_t _keycount;
        Tuple *_arguments; // owned
        TuplePattern *_patterns; // referred
        
    protected:
        State(const emc::Name &name, size_t keycount, Tuple *arguments, TuplePattern *patterns) : Fact(FactType::FACT_STATE), _name(name), _keycount(keycount), _arguments(arguments), _patterns(patterns) {}
        
    public:
        virtual ~State();
        
    public:
        State(const State &) = delete;
        State & operator = (const State &) = delete;
        
    public:
        const emc::Name & name() const { return _name; }
        const Tuple * arguments() const { return _arguments; }
        const TuplePattern * patterns() const { return _patterns; }
        
    public:
        bool validate_pattern(const Unifier &unifier) const;
        
    public:
        Key * key() const;
        Key * key(const Unifier &unifier) const;
        std::string signature(bool key_only) const;
        std::string signature(const Unifier &unifier, bool key_only) const;
        
    public:
        State * clone() const;
        State * map(const Unifier &unifier) const;
        State * rename(emc::Renamer &renamer, emc::Namer &namer);
        
    public:
        bool can_unify(const State *state, Unifier &unifier) const;
        bool can_unify_key(const State *state, Unifier &unifier) const;
        bool can_unify_to(const State *state, Unifier &unifier) const;
        bool can_match(const State *state, const Unifier &unifier) const;
        
    public:
        bool can_unify(const Key *key, Unifier &unifier) const;
        bool can_unify_to(const Key *key, Unifier &unifier) const;
        bool can_unify_by(const Key *key, Unifier &unifier) const;
        
    public:
        size_t depth(const emc::Name &name, const Unifier &unifier) const;
        
    public:
        virtual void subnames(unsigned long mask, const Unifier &unifier, std::set<emc::Name> &names) const;
        virtual void subnames(unsigned long mask, std::set<emc::Name> &names) const;
        
    public:
        bool equal(const State *state) const;
        
    public:
        virtual void info(std::ostream &os) const;
    };
    
    State * make_state(const std::string &name, size_t keycount, Tuple *arguments, TuplePattern *patterns, emc::Namer &namer);
}

#endif /* defined(__symbolic__state__) */
