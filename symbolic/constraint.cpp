//
//  constraint.cpp
//  symbolic
//
//  Created by Li Li on 3/8/15.
//  Copyright (c) 2015 Lilissun. All rights reserved.
//

#include "constraint.h"

#include <sstream>

#include "unifier.h"
#include "term.h"

namespace sym {
    Constraint::~Constraint()
    {
        delete _left;
        delete _right;
    }
    
    Constraint * Constraint::reverse() const
    {
        if (_op == ConstraintType::CONSTRAINT_EQUAL) {
            return make_constraint_not_equal(_left->clone(), _right->clone());
        } else if (_op == ConstraintType::CONSTRAINT_NOT_EQUAL) {
            return make_constraint_equal(_left->clone(), _right->clone());
        } else if (_op == ConstraintType::CONSTRAINT_GREATER) {
            return make_constraint_less_equal(_left->clone(), _right->clone());
        } else if (_op == ConstraintType::CONSTRAINT_GREATER_EQUAL) {
            return make_constraint_less(_left->clone(), _right->clone());
        } else if (_op == ConstraintType::CONSTRAINT_LESS) {
            return make_constraint_greater_equal(_left->clone(), _right->clone());
        } else {
            ASSERT(_op == ConstraintType::CONSTRAINT_LESS_EQUAL);
            return make_constraint_greater(_left->clone(), _right->clone());
        }
    }
    
    Constraint * Constraint::clone() const
    {
        return new Constraint(_left->clone(), _op, _right->clone());
    }
    
    Constraint * Constraint::map(const Unifier &unifier) const
    {
        return new Constraint(_left->map(unifier), _op, _right->map(unifier));
    }
    
    Constraint * Constraint::rename(emc::Renamer &renamer, emc::Namer &namer)
    {
        _left->rename(renamer, namer);
        _right->rename(renamer, namer);
        return this;
    }
    
    void Constraint::subnames(unsigned long mask, const Unifier &unifier, std::set<emc::Name> &names) const
    {
        _left->subnames(mask, unifier, names);
        _right->subnames(mask, unifier, names);
    }
    
    void Constraint::subnames(unsigned long mask, std::set<emc::Name> &names) const
    {
        _left->subnames(mask, names);
        _right->subnames(mask, names);
    }
        
    void Constraint::info(std::ostream &os) const
    {
        _left->info(os);
        os << fact_space << ConstraintTypeName[_op] << fact_space;
        _right->info(os);
    }
    
    Constraint * make_constraint_greater(Term *left, Term *right)
    {
        return new Constraint(left, ConstraintType::CONSTRAINT_GREATER, right);
    }

    Constraint * make_constraint_greater_equal(Term *left, Term *right)
    {
        return new Constraint(left, ConstraintType::CONSTRAINT_GREATER_EQUAL, right);
    }
    
    Constraint * make_constraint_less(Term *left, Term *right)
    {
        return new Constraint(left, ConstraintType::CONSTRAINT_LESS, right);
    }
    
    Constraint * make_constraint_less_equal(Term *left, Term *right)
    {
        return new Constraint(left, ConstraintType::CONSTRAINT_LESS_EQUAL, right);
    }
    
    Constraint * make_constraint_equal(Term *left, Term *right)
    {
        return new Constraint(left, ConstraintType::CONSTRAINT_EQUAL, right);
    }
    
    Constraint * make_constraint_not_equal(Term *left, Term *right)
    {
        return new Constraint(left, ConstraintType::CONSTRAINT_NOT_EQUAL, right);
    }
    
    Constraint * make_constraint(Term *left, ConstraintType type, Term *right)
    {
        return new Constraint(left, type, right);
    }
}