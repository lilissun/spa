//
//  state.cpp
//  calculus
//
//  Created by Li Li on 7/5/15.
//  Copyright (c) 2015 Lilissun. All rights reserved.
//

#include "state.h"

#include "common/debug.h"
#include "symbolic/term.h"
#include "symbolic/name.h"
#include "symbolic/function.h"
#include "symbolic/tuple.h"
#include "symbolic/pair.h"
#include "symbolic/list.h"
#include "symbolic/nonce.h"
#include "symbolic/variable.h"
#include "symbolic/number.h"
#include "symbolic/constant.h"
#include "symbolic/value.h"
#include "symbolic/event.h"
#include "symbolic/guard.h"
#include "symbolic/constraint.h"
#include "time/rule.h"
#include "process.h"
#include "program.h"
#include "context.h"
#include "builder.h"
#include "expression.h"
#include "name.h"
#include "tuple.h"
#include "function.h"
#include "identifier.h"
#include "value.h"

namespace cal {
    State::State(Context *context)
    : Content(context)
    , _identity(nullptr)
    {
        
    }
    
    State::~State()
    {
        for (auto i = _owned_terms.begin(); i != _owned_terms.end(); ++i) {
            delete *i;
        }
        
        for (auto i = _owned_events.begin(); i != _owned_events.end(); ++i) {
            delete *i;
        }
        
        for (auto i = _owned_guards.begin(); i != _owned_guards.end(); ++i) {
            delete *i;
        }
        
        for (auto i = _owned_constraints.begin(); i != _owned_constraints.end(); ++i) {
            delete *i;
        }
    }
    
    State * State::clone() const
    {
        State *state = new State(_context);
        state->_registered_names = _registered_names;
        state->_registered_clocks = _registered_clocks;
        state->_binding_global_timestamps = _binding_global_timestamps;
        state->_identity = _identity;
        state->_history = _history;
        state->_history_tail = _history_tail;
        state->_guards = _guards;
        state->_premises = _premises;
        state->_constraints = _constraints;
        state->_unifier = _unifier;
        return state;
    }
    
    State * State::reset(const State *init) const
    {
        State *state = new State(_context);
        state->_registered_names = init->_registered_names;
        state->_registered_clocks = init->_registered_clocks;
        state->_binding_global_timestamps = init->_binding_global_timestamps;
        state->_identity = _identity;
        state->_history = _history;
        state->_history_tail = _history_tail;
        state->_guards = _guards;
        state->_premises = _premises;
        state->_constraints = _constraints;
        state->_unifier = _unifier;
        return state;
    }
    
    const sym::Name * State::declare_string(const std::string &name, const com::Location &location)
    {
        auto term = sym::make_name(name, _context->namer());
        _owned_terms.push_back(term);
        auto res = _registered_names.insert(std::make_pair(name, std::make_pair(term, location)));
        if (res.second == false) {
            std::stringstream ss;
            ss << name << " declared at " << location;
            ss << " has been declare before at " << res.first->second.second;
            USER_ERROR(ss.str());
        }
        return term;
    }
    
    const sym::Variable * State::declare_variable(const std::string &name, const com::Location &location)
    {
        auto term = sym::make_variable(name, _context->namer());
        _owned_terms.push_back(term);
        auto res = _registered_names.insert(std::make_pair(name, std::make_pair(term, location)));
        if (res.second == false) {
            std::stringstream ss;
            ss << name << " declared at " << location;
            ss << " has been declare before at " << res.first->second.second;
            USER_ERROR(ss.str());
        }
        return term;
    }
    
    const sym::Nonce * State::declare_nonce(const std::string &name, const com::Location &location)
    {
        auto term = sym::make_nonce(name, _context->namer());
        _owned_terms.push_back(term);
        auto res = _registered_names.insert(std::make_pair(name, std::make_pair(term, location)));
        if (res.second == false) {
            std::stringstream ss;
            ss << name << " declared at " << location;
            ss << " has been declare before at " << res.first->second.second;
            USER_ERROR(ss.str());
        }
        return term;
    }
    
    const sym::Number * State::declare_timestamp(const std::string &name, const com::Location &location)
    {
        auto term = sym::make_number(name, _context->namer());
        _owned_terms.push_back(term);
        auto res = _registered_names.insert(std::make_pair(name, std::make_pair(term, location)));
        if (res.second == false) {
            std::stringstream ss;
            ss << name << " declared at " << location;
            ss << " has been declare before at " << res.first->second.second;
            USER_ERROR(ss.str());
        }
        return term;
    }

    const sym::Constant * State::declare_parameter(const std::string &name, const com::Location &location)
    {
        auto term = sym::make_constant(name, _context->namer());
        _owned_terms.push_back(term);
        auto res = _registered_names.insert(std::make_pair(name, std::make_pair(term, location)));
        if (res.second == false) {
            std::stringstream ss;
            ss << name << " declared at " << location;
            ss << " has been declare before at " << res.first->second.second;
            USER_ERROR(ss.str());
        }
        return term;
    }

    const sym::Term * State::get_term(const std::string &name, const com::Location &location) const
    {
        auto it = _registered_names.find(name);
        if (it != _registered_names.end()) {
            return it->second.first;
        } else {
            std::stringstream ss;
            ss << name << " used at " << location;
            ss << " has not been declare before";
            USER_ERROR(ss.str());
        }
    }
    
    void State::init_history()
    {
        auto new_history = _context->program()->new_variable();
        _owned_terms.push_back(new_history);
        _history = new_history;
        _history_tail = new_history;
        _identity = nullptr;
    }
        
    void State::append_history(const sym::Term *term)
    {
        auto new_tail = _context->program()->new_variable();
        unify(_history_tail->clone(), sym::make_pair(term->clone(), new_tail));
        _history_tail = new_tail;
    }
    
    void State::set_history_tail(const sym::Term *tail)
    {
        _history_tail = tail;
    }
    
    void State::add_guard(sym::Guard *guard)
    {
        _owned_guards.push_back(guard);
        _guards.push_back(guard);
    }
    
    void State::add_premise(sym::Event *premise)
    {
        _owned_events.push_back(premise);
        _premises.push_back(premise);
    }
    
    void State::add_constraint(sym::Constraint *constraint)
    {
        _owned_constraints.push_back(constraint);
        _constraints.push_back(constraint);
    }
    
    void State::declare_clock(const std::string &name, const com::Location &location)
    {
        auto timestamp = _context->program()->new_timestamp();
        own_term(timestamp);
        auto res = _registered_clocks.insert(std::make_pair(name, std::make_pair(timestamp, location)));
        if (res.second == false) {
            std::stringstream ss;
            ss << "clock " << name << " declared at " << location;
            ss << " has been declare before at " << res.first->second.second;
            USER_ERROR(ss.str());
        }
    }
    
    const sym::Number * State::get_latest_timestamp(const std::string &clock) const
    {
        auto it = _registered_clocks.find(clock);
        if (it == _registered_clocks.end()) {
            std::stringstream ss;
            ss << "clock " << clock << " has not been declare before";
            USER_ERROR(ss.str());
        } else {
            return it->second.first;
        }
    }
    
    void State::set_latest_timestamp(const std::string &clock, const sym::Number *timestamp)
    {
        auto it = _registered_clocks.find(clock);
        if (it == _registered_clocks.end()) {
            std::stringstream ss;
            ss << "clock " << clock << " has not been declare before";
            USER_ERROR(ss.str());
        } else {
            it->second.first = timestamp;
        }
    }
    
    void State::bind_global_timestamp(const std::string &name, const sym::Number *timestamp)
    {
        auto res = _binding_global_timestamps.insert(std::make_pair(name, timestamp));
        ASSERT(res.second);
    }
    
    const sym::Number * State::get_global_timestamp(const std::string &name)
    {
        auto it = _binding_global_timestamps.find(name);
        ASSERT(it != _binding_global_timestamps.end());
        return it->second;
    }

    
    tim::Rule * State::construct_rule(sym::Event *conclusion)
    {
        _owned_events.push_back(conclusion);
        auto rule = new tim::Rule(conclusion->map(_unifier));
        for (auto i = _guards.begin(); i != _guards.end(); ++i) {
            rule->add_guard((*i)->map(_unifier));
        }
        for (auto i = _premises.begin(); i != _premises.end(); ++i) {
            rule->add_premise((*i)->map(_unifier));
        }
        for (auto i = _constraints.begin(); i != _constraints.end(); ++i) {
            auto constraint = (*i)->map(_unifier);
            rule->add_constraint(constraint);
            delete constraint;
        }
        return rule;
    }
    
    const sym::Term * State::get_identity()
    {
        if (_identity == nullptr) {
            auto program = _context->program();
            auto nonce = program->fresh_nonce();
            
            auto location = program->unique_location();
            auto new_event = program->new_event(nonce, location);
            add_premise(new_event);
            
            auto unique_event = program->unique_event(nonce->clone(), location->clone(), sym::make_list(_history->clone()));
            add_premise(unique_event);
            
            append_history(nonce);

            _identity = nonce;
        }
        return _identity;
    }
    
    void State::set_identity(const sym::Term *term)
    {
        ASSERT(term != nullptr);
        if (_identity == nullptr) {
            _identity = term;
        }
    }
    
    void State::unify(sym::Term *left, sym::Term *right)
    {
        _owned_terms.push_back(left);
        _owned_terms.push_back(right);
        if (left->can_unify(right, _unifier) == false) {
            throw StateInvalid();
        }
    }
    
    void State::unify_to(sym::Term *left, sym::Term *right)
    {
        _owned_terms.push_back(left);
        _owned_terms.push_back(right);
        if (left->can_unify_to(right, _unifier) == false) {
            throw StateInvalid();
        }
    }
    
    void State::info(std::ostream &os) const
    {
        for (auto i = _registered_names.begin(); i != _registered_names.end(); ++i) {
            os << i->first << process_space;
            i->second.first->info(os);
            os << std::endl;
        }
    }
}
