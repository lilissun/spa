//
//  reference.cpp
//  symbolic
//
//  Created by Li Li on 30/3/16.
//  Copyright © 2016 Lilissun. All rights reserved.
//

#include "reference.h"

#include "common/debug.h"
#include "common/runtime.h"
#include "unifier.h"
#include "explicit/namer.h"
#include "explicit/indexer.h"
#include "explicit/renamer.h"
#include "explicit/valuator.h"

namespace sym {
    bool ReferencePattern::match(const Term *term, const Unifier &unifier) const
    {
        return term->type() == TermType::TERM_NUMBER;
    }
    
    void ReferencePattern::info(std::ostream &os) const
    {
        os << term_number_mark;
    }
    
    ReferencePattern * make_reference_pattern()
    {
        return new ReferencePattern();
    }
    
    void Reference::unify(const Term *term, Unifier &unifier) const
    {
        if (term->type() == TermType::TERM_REFERENCE) {
            auto reference = (const Reference *)term;
            // This trick is used to prevent the bi-direction loop between two references
            auto this_mapped_term = unifier.evaluate(_name);
            if (this_mapped_term != nullptr) {
                this_mapped_term->unify(reference, unifier);
            } else if (_name != reference->_name) {
                auto mapped_term = unifier.map(reference->name(), this);
                if (mapped_term != nullptr) {
                    mapped_term->unify(this, unifier);
                }
            }
        } else {
            throw UnifyFailed();
        }
    }
    
    void Reference::unify_to(const Term *term, Unifier &unifier) const
    {
        if (term->type() == TermType::TERM_REFERENCE) {
            auto mapped_term = unifier.map(_name, term);
            if (mapped_term != nullptr && mapped_term->equal(term) == false) {
                throw UnifyFailed();
            }
        } else {
            throw UnifyFailed();
        }
    }
    
    bool Reference::match(const Pattern *pattern, const Unifier &unifier) const
    {
        return pattern == nullptr || pattern->match(this, unifier);
    }
    
    Term * Reference::clone() const
    {
        return new Reference(_name);
    }
    
    Term * Reference::map(const Unifier &unifier) const
    {
        if (unifier.exist(_name) == true) {
            return unifier.evaluate(_name)->map(unifier);
        } else {
            return clone();
        }
    }
    
    Term * Reference::rename(emc::Renamer &renamer, emc::Namer &namer)
    {
        _name = renamer.rename(_name, namer);
        return this;
    }
    
    size_t Reference::depth(const emc::Name &name, const Unifier &unifier) const
    {
        auto term = unifier.evaluate(_name);
        if (term == nullptr) {
            if (_name == name) {
                return 1;
            } else {
                return 0;
            }
        } else {
            return term->depth(name, unifier);
        }
    }
    
    size_t Reference::weight() const
    {
        return TermType::TERM_REFERENCE;
    }
    
    void Reference::subnames(unsigned long mask, const Unifier &unifier, std::set<emc::Name> &names) const
    {
        auto term = unifier.evaluate(_name);
        if (term != nullptr) {
            term->subnames(mask, unifier, names);
        } else {
            subnames(mask, names);
        }
    }
    
    void Reference::subnames(unsigned long mask, std::set<emc::Name> &names) const
    {
        if (mask & TermType::TERM_REFERENCE) {
            names.insert(_name);
        }
    }
    
    void Reference::signature(const Unifier &unifier, std::ostream &os) const
    {
        if (unifier.exist(_name)) {
            unifier.evaluate(_name)->signature(unifier, os);
        } else {
            signature(os);
        }
    }
    
    void Reference::signature(std::ostream &os) const {
        os << _name.id();
    }
    
    emc::Value Reference::evaluate(const emc::Valuator &valuator) const
    {
        return valuator.evaluate(_name);
    }
    
    void Reference::info(std::ostream &os) const
    {
        os << term_reference_mark << _name;
    }
    
    Reference * make_reference(const std::string &name, emc::Namer &namer, emc::Indexer &indexer)
    {
        return new Reference(indexer.variable(name, namer));
    }
    
    Reference * make_reference(const std::string &name, emc::Namer &namer)
    {
        return new Reference(namer.variable(name));
    }
    
    Reference * make_reference(const std::string &name, emc::Name::ID id, emc::Namer &namer)
    {
        return new Reference(namer.variable(name, id));
    }
}