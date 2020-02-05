//
//  constraint.h
//  symbolic
//
//  Created by Li Li on 3/8/15.
//  Copyright (c) 2015 Lilissun. All rights reserved.
//

#ifndef __symbolic__constraint__
#define __symbolic__constraint__

#include <iostream>
#include <vector>
#include <set>

#include "fact.h"

namespace sym {
    enum ConstraintType {
        CONSTRAINT_GREATER,
        CONSTRAINT_GREATER_EQUAL,
        CONSTRAINT_LESS,
        CONSTRAINT_LESS_EQUAL,
        CONSTRAINT_EQUAL,
        CONSTRAINT_NOT_EQUAL,
        CONSTRAINT_TYPE_END,
    };
    
    const std::string ConstraintTypeName[CONSTRAINT_TYPE_END] = {
        ">",
        ">=",
        "<",
        "<=",
        "==",
        "!=",
    };
    
    class Constraint : public Fact {
        
        friend Constraint * make_constraint_greater(Term *left, Term *right);
        friend Constraint * make_constraint_greater_equal(Term *left, Term *right);
        friend Constraint * make_constraint_less(Term *left, Term *right);
        friend Constraint * make_constraint_less_equal(Term *left, Term *right);
        friend Constraint * make_constraint_equal(Term *left, Term *right);
        friend Constraint * make_constraint_not_equal(Term *left, Term *right);
        friend Constraint * make_constraint(Term *left, ConstraintType type, Term *right);
        
    protected:
        Term *_left; // owned
        ConstraintType _op;
        Term *_right; // owned
        
    protected:
        Constraint(Term *left, ConstraintType op, Term *right) : Fact(FactType::FACT_CONSTRAINT), _left(left), _op(op), _right(right) {}
        
    public:
        virtual ~Constraint();
        
    public:
        Constraint(const Constraint &) = delete;
        Constraint & operator = (const Constraint &) = delete;
        
    public:
        const Term * left() const { return _left; }
        ConstraintType op() const { return _op; }
        const Term * right() const { return _right; }
        
    public:
        Constraint * reverse() const;
        void update(ConstraintType op) { _op = op; }
        
    public:
        Constraint * clone() const;
        Constraint * map(const Unifier &unifier) const;
        Constraint * rename(emc::Renamer &renamer, emc::Namer &namer);
        
    public:
        size_t weight() const;
        
    public:
        virtual void subnames(unsigned long mask, const Unifier &unifier, std::set<emc::Name> &names) const;
        virtual void subnames(unsigned long mask, std::set<emc::Name> &names) const;
        
    public:
        virtual void info(std::ostream &os) const;
    };
    
    Constraint * make_constraint_greater(Term *left, Term *right);
    Constraint * make_constraint_greater_equal(Term *left, Term *right);
    Constraint * make_constraint_less(Term *left, Term *right);
    Constraint * make_constraint_less_equal(Term *left, Term *right);
    Constraint * make_constraint_equal(Term *left, Term *right);
    Constraint * make_constraint_not_equal(Term *left, Term *right);
    Constraint * make_constraint(Term *left, ConstraintType type, Term *right);
}

#endif /* defined(__symbolic__constraint__) */
