//
//  key.cpp
//  symbolic
//
//  Created by Li Li on 26/7/14.
//  Copyright (c) 2014 Lilissun. All rights reserved.
//

#include "key.h"


#include <sstream>

#include "common/runtime.h"
#include "unifier.h"
#include "explicit/renamer.h"
#include "term.h"
#include "tuple.h"

namespace sym {
    Key::~Key()
    {
        delete _arguments;
    }
    
    std::string Key::signature() const
    {
        std::stringstream ss;
        ss << _name;
        _arguments->signature(ss);
        return ss.str();
    }
    
    std::string Key::signature(const Unifier &unifier) const
    {
        std::stringstream ss;
        ss << _name;
        _arguments->signature(unifier, ss);
        return ss.str();
    }

    Key * Key::clone() const
    {
        return new Key(_name, _arguments->tuple_clone());
    }
    
    Key * Key::map(const Unifier &unifier) const
    {
        return new Key(_name, _arguments->tuple_map(unifier));
    }
    
    Key * Key::rename(emc::Renamer &renamer, emc::Namer &namer)
    {
        _arguments->rename(renamer, namer);
        return this;
    }

    bool Key::can_unify(const Key * key, Unifier &unifier) const
    {
        if (key->_name != _name) {
            return false;
        }
        ASSERT(key->_arguments->size() == _arguments->size());
        return _arguments->can_unify(key->_arguments, unifier);
    }
    
    bool Key::can_unify_to(const Key * key, Unifier &unifier) const
    {
        if (key->_name != _name) {
            return false;
        }
        ASSERT(key->_arguments->size() == _arguments->size());
        return _arguments->can_unify_to(key->_arguments, unifier);
    }
    
    bool Key::can_match(const Key *key, const Unifier &unifier) const
    {
        Unifier fst_unifier(unifier);
        Unifier snd_unifier(unifier);
        if (_name != key->_name) {
            return false;
        }
        ASSERT(key->_arguments->size() == _arguments->size());
        return _arguments->can_match(key->_arguments, unifier);
    }
    
    void Key::subnames(unsigned long mask, const Unifier &unifier, std::set<emc::Name> &names) const
    {
        _arguments->subnames(mask, unifier, names);
    }
    
    void Key::subnames(unsigned long mask, std::set<emc::Name> &names) const
    {
        _arguments->subnames(mask, names);
    }
        
    void Key::info(std::ostream &os) const
    {
        os << _name;
        _arguments->info(os);
    }
}