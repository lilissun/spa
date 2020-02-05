//
//  program.h
//  calculus
//
//  Created by Li Li on 22/5/15.
//  Copyright (c) 2015 Lilissun. All rights reserved.
//

#ifndef __calculus__program__
#define __calculus__program__

#include <iostream>
#include <map>
#include <set>
#include <vector>
#include <tuple>

#include "content.h"
#include "event.h"
#include "symbolic/constant.h"

namespace sym {
    class Term;
    class Name;
    class Number;
    class Constant;
    class Variable;
    class Function;
    class Nonce;
    class Tuple;
    class Event;
    class Constraint;
}

namespace tim {
    class Rule;
    class Correspondence;
}

namespace cal {
    std::string get_location(size_t location);
    
    const std::string default_network_latency = "pn";
    const std::string default_global_clock = "cg";
    
    class Context;
    class State;
    class Process;
    class Correspondence;
    class Constraint;
    class Function;
    class Tuple;
    class Expression;
    class Identifier;
    class Program : public Content {
    protected:
        State *_init_state; // owned
        Process *_init_process; // owned
        size_t _location;
        bool _shared_clock_rate;
        // if the second expression in _registered_functions is nullptr,
        // then the function result is unchanged comparing to the application.
        // if the third expression in _registered_functions is nullptr,
        // then the function has no requirement for the minimal consumable time.
        // if the fourth bool is true, the function is public; otherwise, it is private.
        //          name?           identifiers        f(...) -> m             time?      public?
        std::map<std::string, std::tuple<Tuple *, Function *, Expression *, Expression *, bool>> _registered_functions; // owned
        std::map<std::string, std::pair<Tuple *, Process *>> _registered_process; // owned
        std::map<std::string, std::pair<EventType, int>> _registered_events;
        //                                 maximum           actual
        std::map<std::string, std::pair<sym::Constant *, sym::Constant *>> _registered_clocks; // owned
        std::set<size_t> _registered_tuples;
        std::vector<std::string> _registered_strings;
        std::vector<std::string> _registered_parameters;
        std::vector<Constraint *> _registered_constraints; // owned
        std::vector<Correspondence *> _registered_queries; // owned
        
    protected:
        std::vector<tim::Rule *> _rules; // owned
        std::vector<sym::Constraint *> _constraints; // owned
        std::vector<tim::Correspondence *> _queries; // owned
        
    public:
        Program(Context * context);
        ~Program();
        
    public:
        State * init_state() const { return _init_state; }
        void init(Process *init_process);
        
    public:
        void execute();
        
    public:
        void register_init_event(const std::string &name, int upperbound = -1);
        void register_join_event(const std::string &name, int upperbound = -1);
        void register_accept_event(const std::string &name);
        void register_function(Tuple *identifiers, Function *function, Expression *result, Expression *time, bool open);
        void register_process(const std::string &name, Tuple *identifiers, Process *process);
        void register_query(Correspondence *correspondence);
        void register_tuple(size_t size) { _registered_tuples.insert(size); }
        void register_string(const std::string &name);
        void register_parameter(const std::string &name);
        void register_constraint(Constraint *constraint);
        void register_clock(const std::string &name, const std::string &maximum_drift);
        void register_clock(const std::string &name, const std::string &maximum_drift, const std::string &shared_drift);
        std::pair<sym::Function *, sym::Term *> function(const std::string &name) const;
        std::pair<Tuple *, Process *> process(const std::string &name) const;
        
    public:
        sym::Name * unique_location();
        sym::Number * new_timestamp() const;
        sym::Constant * network_delay() const;
        sym::Nonce * fresh_nonce() const;
        sym::Variable * new_variable() const;
        
    public:
        bool shared_clock_rate() const { return _shared_clock_rate; }
        sym::Constant * maximum_clock_drift(const std::string &clock) const { return (sym::Constant *) _registered_clocks.at(clock).first->clone(); }
        sym::Constant * constant_clock_drift(const std::string &clock) const { return (sym::Constant *) _registered_clocks.at(clock).second->clone(); }
        
    public:
        sym::Event * know_event(sym::Term *message, sym::Term *timestamp) const;
        sym::Event * new_event(sym::Term *nonce, sym::Term *location) const;
        sym::Event * unique_event(sym::Term *term, sym::Term *location, sym::Term *arguments) const;
        sym::Event * init_event(const std::string &name, sym::Term *term, sym::Term *arguments, sym::Term *timestamp) const;
        sym::Event * accept_event(const std::string &name, sym::Term *term, sym::Term *arguments, sym::Term *timestamp) const;
        sym::Event * join_event(const std::string &name, sym::Term *term, sym::Term *arguments, sym::Term *timestamp) const;
        sym::Event * open_event(sym::Term *message) const;
        sym::Event * leak_event(sym::Term *message) const;
        
    public:
        void output(std::ostream &os) const;
        void info(std::ostream &os) const;
    };
}

#endif /* defined(__calculus__program__) */
