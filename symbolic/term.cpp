//
//  term.cpp
//  symbolic
//
//  Created by Li Li on 18/1/15.
//  Copyright (c) 2015 Lilissun. All rights reserved.
//

#include "term.h"

#include <sstream>

#include "unifier.h"

namespace sym {
    TermSort Term::sort() const {
        TermSort s = _sort();
        switch (s) {
            case TermSort::TERM_SORT_VALUE:
                return TermSort::TERM_SORT_LINEAR_EXPRESSION;
                
            default:
                ASSERT(s != TERM_SORT_END);
                return s;
        }
    }
    
    bool Term::equal(const Term *term, const Unifier &unifier) const
    {
        std::stringstream lhs;
        this->signature(unifier, lhs);
        std::stringstream rhs;
        term->signature(unifier, rhs);
        return lhs.str() == rhs.str();
    }
    
    bool Term::equal(const Term *term) const
    {
        std::stringstream lhs;
        this->signature(lhs);
        std::stringstream rhs;
        term->signature(rhs);
        return lhs.str() == rhs.str();
    }
    
    bool Term::can_unify(const Term *term, Unifier &unifier) const
    {
        try {
            unify(term, unifier);
            return unifier.loopfree();
        } catch (const UnifyFailed &e) {
            return false;
        }
    }
    
    bool Term::can_unify_to(const Term *term, Unifier &unifier) const
    {
        try {
            unify_to(term, unifier);
            return unifier.loopfree();
        } catch (const UnifyFailed &e) {
            return false;
        }
    }
    
    bool Term::can_match(const Term *term, const Unifier &unifier) const
    {
        Unifier fst_unifier(unifier);
        Unifier snd_unifier(unifier);
        return this->can_unify_to(term, fst_unifier) && term->can_unify_to(this, snd_unifier) && fst_unifier.loopfree() && snd_unifier.loopfree();
    }

}