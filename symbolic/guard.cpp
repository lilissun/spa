//
//  guard.cpp
//  symbolic
//
//  Created by Li Li on 18/12/14.
//  Copyright (c) 2014 Lilissun. All rights reserved.
//

#include "guard.h"

#include <sstream>

#include "unifier.h"
#include "term.h"

namespace sym {
    Guard::~Guard()
    {
        delete _left;
        delete _right;
    }
    
//    bool Guard::violated(const Unifier &unifier) const
//    {
////        if (_op == GuardType::GUARD_NOT_EQUAL) {
//            std::stringstream lhs;
//            _left->signature(unifier, lhs);
//            std::stringstream rhs;
//            _right->signature(unifier, rhs);
//            return lhs.str() == rhs.str();
//        } else {
//            ASSERT(_op == GuardType::GUARD_NOT_UNIFIABLE_TO);
//            try {
//                Unifier copy(unifier);
//                _left->unify_to(_right, copy);
//                return false;
//            } catch (const UnifyFailed &e) {
//                return true;
//            }
//        }
//    }
    
    bool Guard::violated() const
    {
        if (_op == GuardType::GUARD_NOT_EQUAL) {
            std::stringstream lhs;
            _left->signature(lhs);
            std::stringstream rhs;
            _right->signature(rhs);
            return lhs.str() == rhs.str();
        } else {
            ASSERT(_op == GuardType::GUARD_NOT_UNIFIABLE_TO);
            try {
                Unifier empty;
                _left->unify_to(_right, empty);
                return false;
            } catch (const UnifyFailed &e) {
                return true;
            }
        }
    }
    
    bool Guard::trivial() const
    {
        if (_op == GuardType::GUARD_NOT_EQUAL) {
            Unifier unifier;
            return _left->can_unify(_right, unifier) == false;
        } else {
            ASSERT(_op == GuardType::GUARD_NOT_UNIFIABLE_TO);
            Unifier unifier;
            return _left->can_unify(_right, unifier) == false;
        }
    }
    
    Guard * Guard::clone() const
    {
        return new Guard(_left->clone(), _op, _right->clone());
    }
    
    Guard * Guard::map(const Unifier &unifier) const
    {
        return new Guard(_left->map(unifier), _op, _right->map(unifier));
    }
    
    Guard * Guard::rename(emc::Renamer &renamer, emc::Namer &namer)
    {
        _left->rename(renamer, namer);
        _right->rename(renamer, namer);
        return this;
    }

    void Guard::subnames(unsigned long mask, const Unifier &unifier, std::set<emc::Name> &names) const
    {
        _left->subnames(mask, unifier, names);
        _right->subnames(mask, unifier, names);
    }
    
    void Guard::subnames(unsigned long mask, std::set<emc::Name> &names) const
    {
        _left->subnames(mask, names);
        _right->subnames(mask, names);
    }
    
    bool Guard::equal(const Guard *guard, const Unifier &unifier) const
    {
        if (_op == GuardType::GUARD_NOT_EQUAL) {
            if (_left->equal(guard->_left, unifier)
                && _right->equal(guard->_right, unifier)) {
                return true;
            }
            
            if (_left->equal(guard->_right, unifier)
                && _right->equal(guard->_left, unifier)) {
                return true;
            }
            
            return false;
        } else {
            ASSERT(_op == GuardType::GUARD_NOT_UNIFIABLE_TO);
            
            if (_left->equal(guard->_left, unifier)
                && _right->equal(guard->_right, unifier)) {
                return true;
            }
            
            return false;
        }
    }
    
    bool Guard::equal(const Guard *guard) const
    {
        if (_op == GuardType::GUARD_NOT_EQUAL) {
            if (_left->equal(guard->_left)
                && _right->equal(guard->_right)) {
                return true;
            }
            
            if (_left->equal(guard->_right)
                && _right->equal(guard->_left)) {
                return true;
            }
            
            return false;
        } else {
            ASSERT(_op == GuardType::GUARD_NOT_UNIFIABLE_TO);
            
            if (_left->equal(guard->_left)
                && _right->equal(guard->_right)) {
                return true;
            }
            
            return false;
        }
    }
    
    void Guard::info(std::ostream &os) const
    {
        _left->info(os);
        os << fact_space << GuardTypeName[_op] << fact_space;
        _right->info(os);
    }
    
    Guard * make_guard_not_equal(Term *left, Term *right)
    {
        return new Guard(left, GuardType::GUARD_NOT_EQUAL, right);
    }
    
    Guard * make_guard_not_unifiable_to(Term *left, Term *right)
    {
        return new Guard(left, GuardType::GUARD_NOT_UNIFIABLE_TO, right);
    }
}