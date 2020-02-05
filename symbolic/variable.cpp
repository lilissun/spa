//
//  variable.cpp
//  symbolic
//
//  Created by Li Li on 24/8/13.
//  Copyright (c) 2013 Lilissun. All rights reserved.
//

#include "variable.h"



#include "explicit/namer.h"
#include "unifier.h"
#include "explicit/indexer.h"
#include "explicit/namer.h"
#include "explicit/renamer.h"
#include "explicit/value.h"
#include "common/debug.h"

namespace sym {
    VariablePattern::~VariablePattern()
    {
        for (auto i = _patterns.begin(); i != _patterns.end(); ++i) {
            delete *i;
        }
    }
    
    bool VariablePattern::match(const Term *term, const Unifier &unifier) const
    {
        for (auto i = _patterns.begin(); i != _patterns.end(); ++i) {
            if (term->match(*i, unifier)) {
                return true;
            }
        }
        return false;
    }
    
    void VariablePattern::info(std::ostream &os) const
    {
        os << term_any_mark;
    }
    
    void Variable::unify(const Term *term, Unifier &unifier) const
    {
        if (term->type() == TermType::TERM_VARIABLE) {
            auto variable = (const Variable *)term;
            // This trick is used to prevent the bi-direction loop between two variables
            auto this_mapped_term = unifier.evaluate(_name);
            if (this_mapped_term != nullptr) {
                this_mapped_term->unify(variable, unifier);
            } else if (_name != variable->_name) {
                auto mapped_term = unifier.map(variable->name(), this);
                if (mapped_term != nullptr) {
                    mapped_term->unify(this, unifier);
                }
            }
        } else if (term->type() == TermType::TERM_CONSTANT) {
            NOT_IMPL;
        } else if (term->type() == TermType::TERM_VALUE) {
            NOT_IMPL;
        } else {
            auto mapped_term = unifier.map(_name, term);
            if (mapped_term != nullptr) {
                mapped_term->unify(term, unifier);
            }
        }
    }
    
    void Variable::unify_to(const Term *term, Unifier &unifier) const
    {
        if (term->type() == TermType::TERM_CONSTANT) {
            NOT_IMPL;
        } else if (term->type() == TermType::TERM_VALUE) {
            NOT_IMPL;
        }
        
        auto mapped_term = unifier.map(_name, term);
        if (mapped_term != nullptr && mapped_term->equal(term) == false) {
            throw UnifyFailed();
        }
    }
    
    bool Variable::match(const Pattern *pattern, const Unifier &unifier) const
    {
        auto term = unifier.evaluate(_name);
        return term == nullptr || pattern == nullptr || term->match(pattern, unifier);
    }
    
    Term * Variable::clone() const
    {
        return new Variable(_name);
    }
    
    Term * Variable::map(const Unifier &unifier) const
    {
        if (unifier.exist(_name)) {
            return unifier.evaluate(_name)->map(unifier);
        } else {
            return clone();
        }
    }
    
    Term * Variable::rename(emc::Renamer &renamer, emc::Namer &namer)
    {
        _name = renamer.rename(_name, namer);
        return this;
    }
    
    size_t Variable::depth(const emc::Name &name, const Unifier &unifier) const
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
    
    size_t Variable::weight() const
    {
        return TermType::TERM_VARIABLE;
    }
    
    void Variable::subnames(unsigned long mask, const Unifier &unifier, std::set<emc::Name> &names) const
    {
        auto term = unifier.evaluate(_name);
        if (term != nullptr) {
            term->subnames(mask, unifier, names);
        } else {
            subnames(mask, names);
        }
    }
    
    void Variable::subnames(unsigned long mask, std::set<emc::Name> &names) const
    {
        if (mask & TermType::TERM_VARIABLE) {
            names.insert(_name);
        }
    }
        
    void Variable::signature(const Unifier &unifier, std::ostream &os) const
    {
        if (unifier.exist(_name)) {
            unifier.evaluate(_name)->signature(unifier, os);
        } else {
            signature(os);
        }
    }
    
    void Variable::signature(std::ostream &os) const {
        os << _name.id();
    }
    
    emc::Value Variable::evaluate(const emc::Valuator &valuator) const
    {
        NOT_IMPL;
    }

    void Variable::info(std::ostream &os) const
    {
        os << _name;
    }
    
    Variable * make_variable(const std::string &name, emc::Namer &namer, emc::Indexer &indexer)
    {
        return new Variable(indexer.variable(name, namer));
    }
    
    Variable * make_variable(const std::string &name, emc::Namer &namer)
    {
        return new Variable(namer.variable(name));
    }
    
    Variable * make_variable(const std::string &name, emc::Name::ID id, emc::Namer &namer)
    {
        return new Variable(namer.variable(name, id));
    }
}