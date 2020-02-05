//
//  state.h
//  calculus
//
//  Created by Li Li on 7/5/15.
//  Copyright (c) 2015 Lilissun. All rights reserved.
//

#ifndef __calculus__state__
#define __calculus__state__

#include <iostream>
#include <vector>

#include "common/exception.h"
#include "common/location.h"
#include "explicit/indexer.h"
#include "symbolic/unifier.h"
#include "content.h"

namespace emc {
    class Namer;
}

namespace sym {
    class Term;
    class Function;
    class Tuple;
    class Pair;
    class List;
    class Name;
    class Nonce;
    class Variable;
    class Number;
    class Constant;
    class Value;
    
    class Guard;
    class Constraint;
    class Event;
}

namespace tim {
    class Rule;
}

namespace cal {
    class Process;
    class StateInvalid : public com::Exception {};
    class State : public Content {
    protected:
        std::vector<sym::Term *> _owned_terms; // owned
        std::vector<sym::Event *> _owned_events; // owned
        std::vector<sym::Guard *> _owned_guards; // owned
        std::vector<sym::Constraint *> _owned_constraints; // owned
        
    protected:
        std::map<std::string, std::pair<const sym::Term *, com::Location>> _registered_names; // referred
        std::map<std::string, std::pair<const sym::Number *, com::Location>> _registered_clocks; // referred
        std::map<std::string, const sym::Number *> _binding_global_timestamps; // refered
        
    protected:
        const sym::Term *_identity; // referred
        
    protected:
        const sym::Term *_history; // referred
        const sym::Term *_history_tail; // referred
        
    protected:
        std::vector<const sym::Guard *> _guards; // referred
        std::vector<const sym::Event *> _premises; // referred
        std::vector<const sym::Constraint *> _constraints; // referred
        
    protected:
        sym::Unifier _unifier;
        
    public:
        State(Context *context);
        virtual ~State();
        
    public:
        State(const State &) = delete;
        State & operator = (const State &) = delete;
        
    public:
        State * clone() const;
        State * reset(const State *init) const;

    public: // weak pointer
        const sym::Name * declare_string(const std::string &name, const com::Location &location);
        const sym::Variable * declare_variable(const std::string &name, const com::Location &location);
        const sym::Nonce * declare_nonce(const std::string &name, const com::Location &location);
        const sym::Number * declare_timestamp(const std::string &name, const com::Location &location);
        const sym::Constant * declare_parameter(const std::string &name, const com::Location &location);
        const sym::Term * get_term(const std::string &name, const com::Location &location) const;
        
    public: // weak pointer
        void declare_clock(const std::string &name, const com::Location &location);
        const sym::Number * get_latest_timestamp(const std::string &clock) const;
        void set_latest_timestamp(const std::string &clock, const sym::Number *timestamp);
        void bind_global_timestamp(const std::string &name, const sym::Number *timestamp);
        const sym::Number * get_global_timestamp(const std::string &name);
        
    public:
        const sym::Term * get_identity();
        const sym::Term * get_raw_identity() { return _identity; }
        void set_identity(const sym::Term *term);
        
    public:
        void init_history();
        void append_history(const sym::Term *term);
        void set_history_tail(const sym::Term *tail);
        const sym::Term * get_history() const { return _history; }
        
    public:
        void own_term(sym::Term *term) { _owned_terms.push_back(term); }
        
    public:
        void add_guard(sym::Guard *guard);
        void add_premise(sym::Event *premise);
        void add_constraint(sym::Constraint *constraint);
        
    public:
        tim::Rule * construct_rule(sym::Event *conclusion);
        
    public:
        void unify(sym::Term *left, sym::Term *right);
        void unify_to(sym::Term *left, sym::Term *right);
        
    public:
        void info(std::ostream &os) const;
    };
}

#endif /* defined(__calculus__state__) */
