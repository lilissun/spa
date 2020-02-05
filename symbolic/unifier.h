//
//  unifier.h
//  symbolic
//
//  Created by Li Li on 24/8/13.
//  Copyright (c) 2013 Lilissun. All rights reserved.
//

#ifndef __symbolic__unifier__
#define __symbolic__unifier__

#include <iostream>
#include <vector>
#include <map>
#include <set>

#include "common/exception.h"
#include "explicit/namer.h"
#include "explicit/order.h"

namespace sym {
    const std::string unifier_space = " ";
    const std::string unifier_substitution_mark = "->";
    
    class Term;
    class UnifyFailed : public com::Exception {};
    class Unifier {
    protected:
        std::map<emc::Name, const Term *> _name_to_term; // referred
        emc::Order _order;
        
    public:
        Unifier() : _name_to_term(), _order() {}
        ~Unifier();
        
    public:
        Unifier(const Unifier &unifier) : _name_to_term(unifier._name_to_term), _order() {}
        Unifier & operator = (const Unifier &unifier) {
            _name_to_term = unifier._name_to_term;
            _order = unifier._order;
            return *this;
        }
        
    public:
        const Term * map(const emc::Name &name, const Term *term);
        const Term * evaluate(const emc::Name &name) const;
        bool exist(const emc::Name &name) const;
        bool loopfree() const { return _order.sortable(); }
        
    public:
        std::map<emc::Name, emc::Name> simple_map(const std::vector<emc::Name> &names) const;
        
    public:
        std::set<emc::Name> keys() const;
        void clear() { _name_to_term.clear(); _order.clear(); }
        
    public:
        void print_info() const;
        friend std::ostream & operator << (std::ostream &os, const Unifier &unifier);
    };
}

#endif /* defined(__symbolic__unifier__) */
