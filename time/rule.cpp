//
//  rule.cpp
//  time
//
//  Created by Li Li on 18/7/14.
//  Copyright (c) 2014 Lilissun. All rights reserved.
//

#include "rule.h"

#include <memory>
#include <vector>
#include <list>
#include <map>

#include <algorithm>

#include "common/debug.h"
#include "common/runtime.h"

#include "explicit/renamer.h"
#include "explicit/name.h"

#include "symbolic/unifier.h"
#include "symbolic/selector.h"
#include "symbolic/event.h"
#include "symbolic/guard.h"
#include "symbolic/constraint.h"
#include "symbolic/term.h"
#include "symbolic/number.h"

#include "context.h"

namespace tim {
    Rule::~Rule()
    {
        for (auto i = _guards.begin(); i != _guards.end(); ++i) {
            delete (*i);
        }
        for (auto i = _premises.begin(); i != _premises.end(); ++i) {
            delete (*i);
        }
        delete _conclusion;
    }
    
    Rule * Rule::rename(emc::Namer &namer)
    {
        emc::Renamer renamer;
        for (auto i = _guards.begin(); i != _guards.end(); ++i) {
            (*i)->rename(renamer, namer);
        }
        for (auto i = _premises.begin(); i != _premises.end(); ++i) {
            (*i)->rename(renamer, namer);
        }
        _conclusion->rename(renamer, namer);
        _zone.rename(renamer);
        return this;
    }
    
    Rule * Rule::clone() const
    {
        Rule *rule = new Rule(_conclusion->clone());
        rule->_select = _select;
        for (auto i = _guards.begin(); i != _guards.end(); ++i) {
            rule->add_guard((*i)->clone());
        }
        for (auto i = _premises.begin(); i != _premises.end(); ++i) {
            rule->add_premise((*i)->clone());
        }
        rule->_zone = _zone;
        return rule;
    }
    
    void Rule::zone_nontrivial() const
    {
        if (_zone.empty()) {
            throw RuleInvalid();
        }
    }
    
    void Rule::conclusion_nontrivial() const
    {
        auto conclusion_key = _conclusion->signature(true);
        for (auto i = _premises.begin(); i != _premises.end(); ++i) {
            if ((*i)->signature(true) == conclusion_key) {
                throw RuleInvalid();
            }
        }
    }
    
    void Rule::guard_satisfiable() const
    {
        for (auto i = _guards.begin(); i != _guards.end(); ++i) {
            if ((*i)->violated()) {
                throw RuleInvalid();
            }
        }
    }
    
    void Rule::remove_trivial_guards()
    {
        auto i = _guards.begin();
        while (i != _guards.end()) {
            if ((*i)->trivial()) {
                delete *i;
                i = _guards.erase(i);
            } else {
                ++i;
            }
        }
    }
    
    void Rule::remove_unrelated_when_provided(const sym::Selector &selector)
    {
        std::vector<bool> _premises_removeable;
        std::set<emc::Name> selectable_ids;
        
        for (auto i = _premises.begin(); i != _premises.end(); ++i) {
            _premises_removeable.push_back(selector.is_removable_event(*i));
        }

        if (selector.is_selectable_event(_conclusion)) {
            _conclusion->subnames(sym::mask_symbol_term_variable, selectable_ids);
        } else {
            _conclusion->subnames(sym::TermType::TERM_VARIABLE | sym::TermType::TERM_NONCE, selectable_ids);
        }
        
        ASSERT(_premises.size() == _premises_removeable.size());
        {
            auto i = _premises.begin();
            auto j = _premises_removeable.begin();
            while (i != _premises.end()) {
                if (*j == false) {
                    (*i)->subnames(sym::TermType::TERM_VARIABLE | sym::TermType::TERM_NONCE, selectable_ids);
                }
                ++i;
                ++j;
            }
        }
        
        bool fixpointed = false;
        while (!fixpointed) {
            fixpointed = true;
            auto i = _premises.begin();
            auto j = _premises_removeable.begin();
            while (i != _premises.end()) {
                if (*j == true) {
                    auto ids = (*i)->get_subnames(sym::mask_symbol_term_variable);
                    std::set<emc::Name> intersection;
                    std::set_intersection(ids.begin(), ids.end(), selectable_ids.begin(), selectable_ids.end(), std::inserter(intersection, intersection.begin()));
                    if (intersection.empty() == false) {
                        fixpointed = false;
                        selectable_ids.insert(ids.begin(), ids.end());
                        *j = false;
                    }
                }
                ++i;
                ++j;
            }
        }
        
        {
            auto i = _premises.begin();
            auto j = _premises_removeable.begin();
            while (i != _premises.end()) {
                if (*j == true) {
                    delete *i;
                    i = _premises.erase(i);
                } else {
                    ++i;
                }
                ++j;
            }
        }
    }
    
    void Rule::merge_premises_with_same_key()
    {
        sym::Unifier unifier;
        std::map<std::string, sym::Event *> keys;
        std::vector<sym::Event *> removeable_premises;
        
        bool merged = true;
        while (merged) {
            merged = false;
            
            auto i = _premises.begin();
            while (i != _premises.end()) {
                std::string key = (*i)->signature(unifier, true);
                auto res = keys.insert(std::make_pair(key, *i));
                if (res.second) {
                    ++i;
                } else if (res.first->second->can_unify(*i, unifier)) {
                    merged = true;
                    removeable_premises.push_back(*i);
                    i = _premises.erase(i);
                } else {
                    for (auto j = removeable_premises.begin(); j != removeable_premises.end(); ++j) {
                        _premises.push_back(*j);
                    }
                    throw RuleInvalid();
                }
            }
            
            keys.clear();
        }
        
        {
            sym::Event *conclusion = _conclusion->map(unifier);
            removeable_premises.push_back(_conclusion);
            _conclusion = conclusion;
        }
        _zone.rename(unifier);
        for (auto i = _premises.begin(); i != _premises.end(); ++i) {
            sym::Event *premise = (*i)->map(unifier);
            removeable_premises.push_back(*i);
            *i = premise;
        }
        for (auto i = _guards.begin(); i != _guards.end(); ++i) {
            sym::Guard *guard = (*i)->map(unifier);
            delete (*i);
            *i = guard;
        }
        
        for (auto i = removeable_premises.begin(); i != removeable_premises.end(); ++i) {
            delete *i;
        }
    }
    
    void Rule::remove_duplicated_guards()
    {
        std::vector<sym::Guard *> guards;
        for (auto i = _guards.begin(); i != _guards.end(); ++i) {
            bool duplicated = false;
            for (auto j = guards.begin(); j != guards.end(); ++j) {
                if ((*i)->equal(*j)) {
                    duplicated = true;
                    break;
                }
            }
            if (duplicated == false) {
                guards.push_back(*i);
            } else {
                delete *i;
            }
        }
        _guards = guards;
    }
    
    void Rule::remove_useless_timestamps_in_zone(const std::set<emc::Name> &parameters)
    {
        std::set<emc::Name> names(parameters);
        for (auto i = _premises.begin(); i != _premises.end(); ++i) {
            (*i)->subnames(sym::TermType::TERM_NUMBER, names);
        }
        _conclusion->subnames(sym::TermType::TERM_NUMBER, names);
        _zone.retain(names);
    }
    
    void Rule::prune_premises(const sym::Pruner &pruner) const
    {
        std::map<emc::Name, size_t> counter;
        for (auto i = _premises.begin(); i != _premises.end(); ++i) {
            auto index = counter.find((*i)->name());
            if (index == counter.end()) {
                auto res = counter.insert(std::make_pair((*i)->name(), 1));
                ASSERT(res.second);
            } else {
                index->second += 1;
            }
        }
        for (auto i = counter.begin(); i != counter.end(); ++i) {
            if (pruner.event_validate(i->first, i->second)) {
                continue;
            } else {
                throw RuleInvalid();
            }
        }
    }
    
    void Rule::sort_premises()
    {
        if (_premises.size() > rule_premise_sort_size) {
            std::vector<std::pair<Index, size_t>> indexes;
            for (auto i = 0; i != _premises.size(); ++i) {
                indexes.push_back(std::make_pair(i, _premises.at(i)->weight()));
            }
            struct ordering {
                bool operator ()(std::pair<Index, size_t> const& a, std::pair<Index, size_t> const& b) {
                    return a.second > b.second;
                }
            };
            std::sort(indexes.begin(), indexes.end(), ordering());
            std::vector<sym::Event *> premises;
            for (auto i = indexes.begin(); i != indexes.end(); ++i) {
                premises.push_back(_premises.at(i->first));
            }
            _premises = premises;
        }
    }
    
    void  Rule::select(const sym::Selector &selector)
    {
        for (auto i = _premises.begin(); i != _premises.end(); ++i) {
            if (selector.is_selectable_event(*i)) {
                _select = std::make_pair(RuleSelect::SELECTABLE, i - _premises.begin());
                return ;
            }
        }
        if (selector.is_selectable_event(_conclusion)) {
            _select = std::make_pair(RuleSelect::NONSELECTABLE, 0);
            return ;
        } else {
            _select = std::make_pair(RuleSelect::FINAL, 0);
            return ;
        }
    }
    
    Rule * Rule::add_constraint(const sym::Constraint *constraint)
    {
        _zone.update(constraint);
        return this;
    }
    
    bool Rule::stricter_than_1st_step(const std::vector<sym::Event *>::const_iterator &rule_premise, const Rule *rule, const sym::Unifier &unifier) const
    {
        if (rule_premise == rule->_premises.end()) {
            sym::Unifier copy(unifier);
            return stricter_than_2nd_step(rule->_guards.begin(), rule, copy);
        }
        
        for (auto this_premise = _premises.begin(); this_premise != _premises.end(); ++this_premise) {
            sym::Unifier copy(unifier);
            if ((*rule_premise)->can_unify_to(*this_premise, copy)) {
                if(stricter_than_1st_step(std::next(rule_premise), rule, copy)) {
                    return true;
                }
            }
        }
        return false;
    }
    
    bool Rule::stricter_than_2nd_step(const std::vector<sym::Guard *>::const_iterator &rule_guard, const Rule *rule, const sym::Unifier &unifier) const
    {
        if (rule_guard == rule->_guards.end()) {
            ppl::Zone zone_copy = rule->_zone;
            zone_copy.rename(unifier);
            return zone_copy.contain(_zone);
        }
        
        for (auto this_guard = _guards.begin(); this_guard != _guards.end(); ++this_guard) {
            if ((*rule_guard)->equal(*this_guard, unifier)) {
                if(stricter_than_2nd_step(std::next(rule_guard), rule, unifier)) {
                    return true;
                }
            }
        }
        return false;
    }
    
    Rule * Rule::compose(const Rule *rule, emc::Namer &namer) const
    {
        ASSERT(this->_select.first == RuleSelect::NONSELECTABLE);
        ASSERT(rule->_select.first == RuleSelect::SELECTABLE);
        sym::Unifier unifier;
        if (_conclusion->can_unify(rule->premise(rule->_select.second), unifier)) {
            Rule *ret = new Rule(rule->_conclusion->map(unifier));
            for (auto i = _premises.begin(); i != _premises.end(); ++i) {
                ret->add_premise((*i)->map(unifier));
            }
            for (auto i = rule->_premises.begin(); i != rule->_premises.end(); ++i) {
                if (std::distance(rule->_premises.begin(), i) == rule->_select.second) {
                    continue;
                } else {
                    ret->add_premise((*i)->map(unifier));
                }
            }
            for (auto i = _guards.begin(); i != _guards.end(); ++i) {
                ret->add_guard((*i)->map(unifier));
            }
            for (auto i = rule->_guards.begin(); i != rule->_guards.end(); ++i) {
                ret->add_guard((*i)->map(unifier));
            }
            ret->_zone = _zone;
            ret->_zone.intersect(rule->_zone);
            ret->_zone.rename(unifier);
            ret->rename(namer);
            return ret;
        } else {
            return nullptr;
        }
    }
    
    bool Rule::imply(const Rule *rule) const
    {
        sym::Unifier unifier;
        if(_conclusion->can_unify_to(rule->_conclusion, unifier)) {
            return rule->stricter_than_1st_step(_premises.begin(), this, unifier);
        } else {
            return false;
        }
    }
    
    void Rule::info(std::ostream &os) const
    {
        os << rule_mark << rule_mark_left << std::endl;
        for (auto i = _guards.begin(); i != _guards.end(); ++i) {
            os << rule_tab << rule_guard_mark << rule_space;
            (*i)->info(os);
            os << rule_mark_separator << std::endl;
        }
        for (auto i = _premises.begin(); i != _premises.end(); ++i) {
            os << rule_tab << rule_premise_mark << rule_space;
            (*i)->info(os);
            os << rule_mark_separator << std::endl;
        }
        os << rule_tab << rule_conclusion_mark << rule_space;
        _conclusion->info(os);
        os << rule_mark_separator << std::endl;
        auto constraints = _zone.info();
        for (auto constraint = constraints.begin(); constraint != constraints.end(); ++constraint) {
            os << rule_tab << rule_constraint_mark << rule_space << *constraint << rule_mark_separator << std::endl;
        }
        os << rule_mark_right << rule_mark_separator << std::endl;
    }
}