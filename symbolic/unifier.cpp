//
//  unifier.cpp
//  symbolic
//
//  Created by Li Li on 24/8/13.
//  Copyright (c) 2013 Lilissun. All rights reserved.
//

#include "unifier.h"

#include <set>

#include "common/runtime.h"
#include "explicit/name.h"
#include "term.h"
#include "nonce.h"
#include "function.h"
#include "name.h"
#include "number.h"
#include "value.h"
#include "constant.h"
#include "tuple.h"
#include "variable.h"
#include "explicit/toposort.h"

namespace sym {    
    Unifier::~Unifier()
    {

    }
    
    const Term * Unifier::map(const emc::Name &name, const Term *term)
    {
        if (_order.sortable() == false) {
            return nullptr;
        }
        
        auto res = _name_to_term.insert(std::make_pair(name, term));
        if (res.second) {
            std::set<emc::Name> term_subnames;
            term->subnames(mask_symbol_term_variable, term_subnames);
            _order.insert(name, term_subnames);
            return nullptr;
        } else {
            return res.first->second;
        }
    }
    
    const Term * Unifier::evaluate(const emc::Name &name) const
    {
        if (_order.sortable() == false) {
            return nullptr;
        }
        
        auto it = _name_to_term.find(name);
        if (it == _name_to_term.end()) {
            return nullptr;
        } else {
            return it->second;
        }
    }
    
    bool Unifier::exist(const emc::Name &name) const
    {
        return _name_to_term.find(name) != _name_to_term.end();
    }
   
    std::set<emc::Name> Unifier::keys() const
    {
        std::set<emc::Name> names;
        for (auto i = _name_to_term.begin(); i != _name_to_term.end(); ++i) {
            names.insert(i->first);
        }
        return names;
    }
    
    std::map<emc::Name, emc::Name> Unifier::simple_map(const std::vector<emc::Name> &names) const
    {
        ASSERT(_order.sortable());
        std::map<emc::Name, emc::Name> name_map;
        for (auto i = names.begin(); i != names.end(); ++i) {
            emc::Name name(*i);
            while (true) {
                auto it = evaluate(name);
                if (it == nullptr) {
                    name_map.insert(std::make_pair(*i, name));
                    break;
                } else {
                    name = it->name();
                }
            }
        }
        return name_map;
    }
    
    void Unifier::print_info() const
    {
        std::cout << *this;
    }
    
    std::ostream & operator << (std::ostream &os, const Unifier &unifier)
    {
        for (auto i = unifier._name_to_term.begin(); i != unifier._name_to_term.end(); ++i) {
            os << i->first;
            os << unifier_space << unifier_substitution_mark << unifier_space;
            i->second->info(os);
            os << std::endl;
        }
        return os;
    }
}