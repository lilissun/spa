//
//  nonce.cpp
//  symbolic
//
//  Created by Li Li on 22/8/13.
//  Copyright (c) 2013 Lilissun. All rights reserved.
//

#include "nonce.h"

#include "common/debug.h"
#include "unifier.h"
#include "explicit/namer.h"
#include "explicit/indexer.h"
#include "explicit/renamer.h"
#include "explicit/value.h"

namespace sym {
    bool NoncePattern::match(const Term *term, const Unifier &unifier) const
    {
        return term->type() == TermType::TERM_NONCE;
    }
    
    void NoncePattern::info(std::ostream &os) const
    {
        os << term_nonce_mark_left << term_any_mark << term_nonce_mark_right;
    }
    
    void Nonce::unify(const Term *term, Unifier &unifier) const
    {
        if (term->type() == TermType::TERM_NONCE) {
            auto nonce = (const Nonce *)term;
            // This trick is used to prevent the bi-direction loop between two nonces
            auto this_mapped_term = unifier.evaluate(_name);
            if (this_mapped_term != nullptr) {
                this_mapped_term->unify(nonce, unifier);
            } else if (_name != nonce->_name) {
                auto term_mapped_term = unifier.map(nonce->name(), this);
                if (term_mapped_term != nullptr) {
                    term_mapped_term->unify(this, unifier);
                }
            }
        } else if (term->type() == TermType::TERM_VARIABLE) {
            return term->unify(this, unifier);
        } else if (term->type() == TermType::TERM_CONSTANT) {
            NOT_IMPL;
        } else if (term->type() == TermType::TERM_VALUE) {
            NOT_IMPL;
        } else {
            throw UnifyFailed();
        }
    }
    
    void Nonce::unify_to(const Term *term, Unifier &unifier) const
    {
        if (term->type() == TermType::TERM_NONCE) {
            auto mapped_term = unifier.map(_name, term);
            if (mapped_term != nullptr && mapped_term->equal(term) == false) {
                throw UnifyFailed();
            }
        } else if (term->type() == TermType::TERM_CONSTANT) {
            NOT_IMPL;
        } else if (term->type() == TermType::TERM_VALUE) {
            NOT_IMPL;
        } else {
            throw UnifyFailed();
        }
    }
    
    bool Nonce::match(const Pattern *pattern, const Unifier &unifier) const
    {
        return pattern == nullptr || pattern->match(this, unifier);
    }
    
    Term * Nonce::clone() const
    {
        return new Nonce(_name);
    }
    
    Term * Nonce::map(const Unifier &unifier) const
    {
        if (unifier.exist(_name)) {
            return unifier.evaluate(_name)->map(unifier);
        } else {
            return clone();
        }
    }
    
    Term * Nonce::rename(emc::Renamer &renamer, emc::Namer &namer)
    {
        _name = renamer.rename(_name, namer);
        return this;
    }
    
    size_t Nonce::depth(const emc::Name &name, const Unifier &unifier) const
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
    
    size_t Nonce::weight() const
    {
        return TermType::TERM_NONCE;
    }
    
    void Nonce::subnames(unsigned long mask, const Unifier &unifier, std::set<emc::Name> &names) const
    {
        auto term = unifier.evaluate(_name);
        if (term != nullptr) {
            term->subnames(mask, unifier, names);
        } else {
            subnames(mask, names);
        }
    }

    void Nonce::subnames(unsigned long mask, std::set<emc::Name> &names) const
    {
        if (mask & TermType::TERM_NONCE) {
            names.insert(_name);
        }
        return ;
    }
    
    void Nonce::signature(const Unifier &unifier, std::ostream &os) const
    {
        if (unifier.exist(_name)) {
            unifier.evaluate(_name)->signature(unifier, os);
        } else {
            signature(os);
        }
    }
    
    void Nonce::signature(std::ostream &os) const
    {
        os << _name.id();
    }
    
    emc::Value Nonce::evaluate(const emc::Valuator &valuator) const
    {
        NOT_IMPL;
    }

    void Nonce::info(std::ostream &os) const
    {
        os << term_nonce_mark_left << _name << term_nonce_mark_right;
    }
    
    Nonce * make_nonce(const std::string &name, emc::Namer &namer, emc::Indexer &indexer)
    {
        return new Nonce(indexer.variable(name, namer));
    }
    
    Nonce * make_nonce(const std::string &name, emc::Namer &namer)
    {
        return new Nonce(namer.variable(name));
    }
    
    Nonce * make_nonce(const std::string &name, emc::Name::ID id, emc::Namer &namer)
    {
        return new Nonce(namer.variable(name, id));
    }
}