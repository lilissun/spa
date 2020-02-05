//
//  event.cpp
//  symbolic
//
//  Created by Li Li on 24/8/13.
//  Copyright (c) 2013 Lilissun. All rights reserved.
//

#include "event.h"

#include <sstream>


#include "common/runtime.h"
#include "unifier.h"
#include "term.h"
#include "selector.h"
#include "tuple.h"
#include "key.h"

namespace sym {
    Event::~Event()
    {
        delete _arguments;
    }
    
    Key * Event::key() const
    {
        return new Key(_name, _arguments->tuple_clone(_keycount));
    }
    
    Key * Event::key(const Unifier &unifier) const
    {
        return new Key(_name, _arguments->tuple_map(_keycount, unifier));
    }
    
    std::string Event::signature(bool key_only) const
    {
        auto size = key_only ? _keycount : _arguments->size();
        std::stringstream ss;
        ss << _name;
        _arguments->tuple_signature(size, ss);
        return ss.str();
    }
    
    std::string Event::signature(const Unifier &unifier, bool key_only) const
    {
        auto size = key_only ? _keycount : _arguments->size();
        std::stringstream ss;
        ss << _name;
        _arguments->tuple_signature(size, unifier, ss);
        return ss.str();
    }
    
    Event * Event::clone() const
    {
        return new Event(_name, _keycount, _arguments->tuple_clone());
    }
    
    Event * Event::map(const Unifier &unifier) const
    {
        return new Event(_name, _keycount, _arguments->tuple_map(unifier));
    }
    
    Event * Event::rename(emc::Renamer &renamer, emc::Namer &namer)
    {
        _arguments->rename(renamer, namer);
        return this;
    }
    
    size_t Event::weight() const
    {
        return _arguments->weight();
    }
    
    bool Event::can_unify(const Event * event, Unifier &unifier) const
    {
        if (event->_name != _name) {
            return false;
        }
        ASSERT(event->_arguments->size() == _arguments->size());
        return _arguments->can_unify(event->_arguments, unifier);
    }
    
    bool Event::can_unify_to(const Event * event, Unifier &unifier) const
    {
        if (event->_name != _name) {
            return false;
        }
        ASSERT(event->_arguments->size() == _arguments->size());
        return _arguments->can_unify_to(event->_arguments, unifier);
    }
    
    bool Event::can_match(const Event * event, const Unifier &unifier) const
    {
        Unifier fst_unifier(unifier);
        Unifier snd_unifier(unifier);
        if (event->_name != _name) {
            return false;
        }
        ASSERT(event->_arguments->size() == _arguments->size());
        return _arguments->can_match(event->_arguments, unifier);
    }
    
    void Event::subnames(unsigned long mask, const Unifier &unifier, std::set<emc::Name> &names) const
    {
        _arguments->subnames(mask, unifier, names);
    }
    
    void Event::subnames(unsigned long mask, std::set<emc::Name> &names) const
    {
        _arguments->subnames(mask, names);
    }
    
    void Event::info(std::ostream &os) const
    {
        os << _name;
//        os << fact_keycount_separator << _keycount;
        _arguments->info(os);
    }
    
    Event * make_event(const std::string &name, size_t keycount, Tuple *arguments, emc::Namer &namer)
    {
        return new Event(namer.constant(name), keycount, arguments);
    }
}