//
//  guard.h
//  symbolic
//
//  Created by Li Li on 18/12/14.
//  Copyright (c) 2014 Lilissun. All rights reserved.
//

#ifndef __symbolic__guard__
#define __symbolic__guard__

#include <iostream>
#include <vector>
#include <set>

#include "fact.h"
#include "explicit/name.h"

namespace sym {
    // Here we only deal with specific guards,
    // i.e., 'not equal' and 'not unifiable to'.
    // Maybe we can add more later.
    enum GuardType {
        GUARD_NOT_EQUAL,
        GUARD_NOT_UNIFIABLE_TO,
        GUARD_TYPE_END,
    };
    
    const std::string GuardTypeName[GUARD_TYPE_END] = {
        "!=",
        "!~>",
    };
    
    class Guard : public Fact {
        
        friend Guard * make_guard_not_equal(Term *left, Term *right);
        friend Guard * make_guard_not_unifiable_to(Term *left, Term *right);
        
    protected:
        Term *_left;
        GuardType _op;
        Term *_right;
        
    protected:
        Guard(Term *left, GuardType op, Term *right) : Fact(FactType::FACT_GUARD), _left(left), _op(op), _right(right) {}
        
    public:
        virtual ~Guard();
        
    public:
        Guard(const Guard &) = delete;
        Guard & operator = (const Guard &) = delete;
        
    public:
//        bool violated(const Unifier &unifier) const;
        bool violated() const;
        bool trivial() const;
        
    public:
        const Term * left() const { return _left; }
        GuardType op() const { return _op; }
        const Term * right() const { return _right; }
        
    public:
        Guard * clone() const;
        Guard * map(const Unifier &unifier) const;
        Guard * rename(emc::Renamer &renamer, emc::Namer &namer);
        
    public:
        virtual void subnames(unsigned long mask, const Unifier &unifier, std::set<emc::Name> &names) const;
        virtual void subnames(unsigned long mask, std::set<emc::Name> &names) const;
        
    public:
        bool equal(const Guard *guard, const Unifier &unifier) const;
        bool equal(const Guard *guard) const;
        
    public:
        virtual void info(std::ostream &os) const;
    };
    
    Guard * make_guard_not_equal(Term *left, Term *right);
    Guard * make_guard_not_unifiable_to(Term *left, Term *right);
}

#endif /* defined(__symbolic__guard__) */
