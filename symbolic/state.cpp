//
//  state.cpp
//  symbolic
//
//  Created by Li Li on 31/10/13.
//  Copyright (c) 2013 Lilissun. All rights reserved.
//

#include "state.h"

#include <sstream>

#include "common/debug.h"
#include "common/runtime.h"
#include "unifier.h"
#include "term.h"
#include "tuple.h"
#include "key.h"

namespace sym {
    State::~State()
    {
        delete _arguments;
    }
    
    bool State::validate_pattern(const Unifier &unifier) const
    {
        return _arguments->match(_patterns, unifier) == false;
    }
    
    Key * State::key() const
    {
        return new Key(_name, _arguments->tuple_clone(_keycount));
    }
    
    Key * State::key(const Unifier &unifier) const
    {
        return new Key(_name, _arguments->tuple_map(_keycount, unifier));
    }
    
    std::string State::signature(bool key_only) const
    {
        auto size = key_only ? _keycount : _arguments->size();
        std::stringstream ss;
        ss << _name;
        _arguments->tuple_signature(size, ss);
        return ss.str();
    }
    
    std::string State::signature(const Unifier &unifier, bool key_only) const
    {
        auto size = key_only ? _keycount : _arguments->size();
        std::stringstream ss;
        ss << _name;
        _arguments->tuple_signature(size, unifier, ss);
        return ss.str();
    }
    
    State * State::clone() const
    {
        return new State(_name, _keycount, _arguments->tuple_clone(), _patterns);
    }
    
    State * State::map(const Unifier &unifier) const
    {
        return new State(_name, _keycount, _arguments->tuple_map(unifier), _patterns);
    }
    
    State * State::rename(emc::Renamer &renamer, emc::Namer &namer)
    {
        _arguments->rename(renamer, namer);
        return this;
    }
    
    bool State::can_unify(const State * state, Unifier &unifier) const
    {
        if (state->_name != _name) {
            return false;
        }
        
        ASSERT(state->_arguments->size() == _arguments->size());
        return _arguments->can_unify(state->_arguments, unifier);
    }
    
    bool State::can_unify_key(const State *state, Unifier &unifier) const
    {
        if (state->_name != _name) {
            return false;
        }
        
        ASSERT(_keycount <= _arguments->size());
        ASSERT(_arguments->size() == state->_arguments->size());
        ASSERT(_keycount == state->_keycount);
        return _arguments->tuple_can_unify(_keycount, state->_arguments, unifier);
    }

    bool State::can_unify_to(const State * state, Unifier &unifier) const
    {
        if (state->_name != _name) {
            return false;
        }
        
        ASSERT(state->_arguments->size() == _arguments->size());
        return _arguments->can_unify_to(state->_arguments, unifier);
    }
    
    bool State::can_match(const State *state, const Unifier &unifier) const
    {
        if (state->_name != _name) {
            return false;
        }
        
        Unifier fst_unifier(unifier);
        Unifier snd_unifier(unifier);
        ASSERT(state->_arguments->size() == _arguments->size());
        return _arguments->can_match(state->_arguments, unifier);
    }
    
    bool State::can_unify(const Key *key, Unifier &unifier) const
    {
        if (key->name() != _name) {
            return false;
        }
        
        ASSERT(_keycount <= _arguments->size());
        ASSERT(_keycount == key->arguments()->size());
        return _arguments->tuple_can_unify(_keycount, key->arguments(), unifier);
    }
    
    bool State::can_unify_to(const Key *key, Unifier &unifier) const
    {
        if (key->name() != _name) {
            return false;
        }
        
        ASSERT(_keycount <= _arguments->size());
        ASSERT(_keycount == key->arguments()->size());
        return _arguments->tuple_can_unify_to(_keycount, key->arguments(), unifier);
    }
    
    bool State::can_unify_by(const Key *key, Unifier &unifier) const
    {
        if (key->name() != _name) {
            return false;
        }
        
        ASSERT(_keycount <= _arguments->size());
        ASSERT(_keycount == key->arguments()->size());
        return key->arguments()->tuple_can_unify_to(_keycount, _arguments, unifier);
    }
    
    size_t State::depth(const emc::Name &name, const Unifier &unifier) const
    {
        return _arguments->depth(name, unifier);
    }
    
    void State::subnames(unsigned long mask, const Unifier &unifier, std::set<emc::Name> &names) const
    {
        _arguments->subnames(mask, unifier, names);
    }
    
    void State::subnames(unsigned long mask, std::set<emc::Name> &names) const
    {
        _arguments->subnames(mask, names);
    }
        
    bool State::equal(const State *state) const
    {
        if (signature(false) == state->signature(false)) {
            return true;
        } else {
            return false;
        }
    }
    
    void State::info(std::ostream &os) const
    {
        os << _name << fact_keycount_separator << _keycount;
        _arguments->info(os);
    }

    State * make_state(const std::string &name, size_t keycount, Tuple *arguments, TuplePattern *patterns, emc::Namer &namer)
    {
        return new State(namer.constant(name), keycount, arguments, patterns);
    }
}
