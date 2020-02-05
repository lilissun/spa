//
//  query.cpp
//  time
//
//  Created by Li Li on 5/8/15.
//  Copyright (c) 2015 Lilissun. All rights reserved.
//

#include "query.h"

#include <memory>
#include <vector>
#include <list>
#include <map>

#include <algorithm>

#include "common/debug.h"
#include "common/runtime.h"

#include "explicit/renamer.h"
#include "explicit/namer.h"
#include "explicit/name.h"

#include "symbolic/unifier.h"
#include "symbolic/selector.h"
#include "symbolic/event.h"
#include "symbolic/guard.h"
#include "symbolic/constraint.h"
#include "symbolic/term.h"
#include "symbolic/number.h"
#include "symbolic/tuple.h"

#include "context.h"

namespace tim {
    Correspondence::~Correspondence()
    {
        delete _injective;
        for (auto i = _premises.begin(); i != _premises.end(); ++i) {
            delete *i;
        }
        delete _conclusion;
    }
    
    Correspondence * Correspondence::add_constraint(const sym::Constraint *constraint)
    {
        _zone.update(constraint);
        return this;
    }
    
    ppl::ZoneSet Correspondence::make_rule_stricter(const std::vector<sym::Event *>::const_iterator &this_premise, const Rule *rule, const std::set<emc::Name> &parameters, const sym::Unifier &unifier) const
    {
        if (this_premise == _premises.end()) {
            ppl::Zone copy(_zone);
            copy.rename(unifier);
            return copy.make_contain(rule->zone(), parameters);
        }
        
        ppl::ZoneSet zones(false);
        for (size_t i = 0; i != rule->premise_size(); ++i) {
            sym::Unifier copy(unifier);
            if ((*this_premise)->can_unify_to(rule->premise(i), copy)) {
                zones.append(make_rule_stricter(std::next(this_premise), rule, parameters, copy));
            }
        }
        return zones;
    }
    
    ppl::ZoneSet Correspondence::ensure_non_injective_correspondence(const Rule *rule, const std::set<emc::Name> &parameters) const
    {
        sym::Unifier unifier;
        if (_conclusion->can_unify_to(rule->conclusion(), unifier)) {
            auto zones = rule->zone().make_empty(parameters);
            if (_injective != nullptr) {
                for (size_t i = 0; i != rule->premise_size(); ++i) {
                    sym::Unifier copy(unifier);
                    if (_injective->can_unify_to(rule->premise(i), copy)) {
                        zones.append(make_rule_stricter(_premises.begin(), rule, parameters, copy));
                    }
                }
            } else {
                zones.append(make_rule_stricter(_premises.begin(), rule, parameters, unifier));
            }
            return zones;
        } else {
            return ppl::ZoneSet(true);
        }
    }
    
    bool Correspondence::check_rule_premise_stricter(const std::vector<sym::Event *>::const_iterator &this_premise, const Rule *rule, const sym::Unifier &unifier) const
    {
        if (this_premise == _premises.end()) {
            return true;
        }
        
        for (size_t i = 0; i != rule->premise_size(); ++i) {
            sym::Unifier copy(unifier);
            if ((*this_premise)->can_unify_to(rule->premise(i), copy)) {
                if (check_rule_premise_stricter(std::next(this_premise), rule, copy)) {
                    return true;
                }
            }
        }
        return false;
    }
    
    std::vector<size_t> Correspondence::find_injective_correspondence(const Rule *rule) const
    {
        sym::Unifier unifier;
        std::vector<size_t> injectives;
        if (_conclusion->can_unify_to(rule->conclusion(), unifier)) {
            for (size_t rule_premise = 0; rule_premise != rule->premise_size(); ++rule_premise) {
                sym::Unifier copy(unifier);
                if (_injective->can_unify_to(rule->premise(rule_premise), copy)) {
                    if (check_rule_premise_stricter(_premises.begin(), rule, copy)) {
                        injectives.push_back(rule_premise);
                    }
                }
            }
        }
        return injectives;
    }
    
    ppl::ZoneSet Correspondence::ensure_injective_correspondence(const Rule *_left_rule, const Rule *_right_rule, const std::set<emc::Name> &parameters, emc::Namer &namer) const
    {
        ASSERT(_injective != nullptr);
        
        const Rule *left_rule = _left_rule;
        const Rule *right_rule = (_left_rule == _right_rule) ? _right_rule->clone()->rename(namer) : _right_rule;
        
        sym::Unifier unifier;
        {
            std::vector<size_t> left_injectives = find_injective_correspondence(left_rule);
            if (left_injectives.empty()) {
                if (_left_rule == _right_rule) { delete right_rule; }
                return ppl::ZoneSet(true);
            }
            for (auto premise = left_injectives.begin(); premise != left_injectives.end(); ++premise) {
                if (_injective->can_unify(left_rule->premise(*premise), unifier) == false) {
                    if (_left_rule == _right_rule) { delete right_rule; }
                    return ppl::ZoneSet(true);
                }
            }
        }
        {
            std::vector<size_t> right_injectives = find_injective_correspondence(right_rule);
            if (right_injectives.empty()) {
                if (_left_rule == _right_rule) { delete right_rule; }
                return ppl::ZoneSet(true);
            }
            for (auto premise = right_injectives.begin(); premise != right_injectives.end(); ++premise) {
                if (_injective->can_unify(right_rule->premise(*premise), unifier) == false) {
                    if (_left_rule == _right_rule) { delete right_rule; }
                    return ppl::ZoneSet(true);
                }
            }
        }
        
        auto rule_conclusion_tuple = sym::make_tuple();
        rule_conclusion_tuple->append(left_rule->conclusion()->arguments()->tuple_map(left_rule->conclusion()->keycount(), unifier));
        rule_conclusion_tuple->append(right_rule->conclusion()->arguments()->tuple_map(right_rule->conclusion()->keycount(), unifier));
        auto rule = new Rule(sym::make_event("equal", 2, rule_conclusion_tuple, namer));
        for (size_t left_guard = 0; left_guard != left_rule->guard_size(); ++left_guard) {
            rule->add_guard(left_rule->guard(left_guard)->map(unifier));
        }
        for (size_t right_guard = 0; right_guard != right_rule->guard_size(); ++right_guard) {
            rule->add_guard(right_rule->guard(right_guard)->map(unifier));
        }
        for (size_t left_premise = 0; left_premise != left_rule->premise_size(); ++left_premise) {
            rule->add_premise(left_rule->premise(left_premise)->map(unifier));
        }
        for (size_t right_premise = 0; right_premise != right_rule->premise_size(); ++right_premise) {
            rule->add_premise(right_rule->premise(right_premise)->map(unifier));
        }
        rule->_zone.intersect(left_rule->zone());
        rule->_zone.intersect(right_rule->zone());
        rule->_zone.rename(unifier);
        try {
            rule->merge_premises_with_same_key();
            rule->zone_nontrivial();
            rule->guard_satisfiable();
            
            ASSERT(rule->conclusion()->arguments()->size() == 2);
            std::stringstream left_ss;
            rule->conclusion()->arguments()->at(0)->signature(left_ss);
            std::stringstream right_ss;
            rule->conclusion()->arguments()->at(1)->signature(right_ss);
            
            if (left_ss.str() == right_ss.str()) {
                if (_left_rule == _right_rule) { delete right_rule; }
                delete rule;
                return ppl::ZoneSet(true);
            } else {
                auto ret = rule->_zone.make_empty(parameters);
                if (_left_rule == _right_rule) { delete right_rule; }
                delete rule;
                return ret;
            }
        } catch (const RuleInvalid &e) {
            if (_left_rule == _right_rule) { delete right_rule; }
            delete rule;
            return ppl::ZoneSet(true);
        }
    }

    ppl::ZoneSet Correspondence::check_rule_stricter(const std::vector<sym::Event *>::const_iterator &this_premise, const Rule *rule, const ppl::Zone &zone, const std::set<emc::Name> &parameters, const sym::Unifier &unifier) const
    {
        ppl::ZoneSet ret(false);
        if (this_premise == _premises.end()) {
            ppl::Zone this_zone(_zone);
            this_zone.intersect(zone);
            this_zone.rename(unifier);
            this_zone.intersect(rule->zone());
            this_zone.retain(parameters);
            ret.insert(this_zone);
            return ret;
        }
        
        for (size_t i = 0; i != rule->premise_size(); ++i) {
            sym::Unifier copy(unifier);
            if ((*this_premise)->can_unify_to(rule->premise(i), copy)) {
                ret.append(check_rule_stricter(std::next(this_premise), rule, zone, parameters, copy));
            }
        }
        return ret;
    }
    
    ppl::ZoneSet Correspondence::keep_non_injective_correspondence(const Rule *rule, const ppl::Zone &zone, const std::set<emc::Name> &parameters) const
    {
        sym::Unifier unifier;
        ppl::ZoneSet ret(false);
        if(_conclusion->can_unify_to(rule->conclusion(), unifier)) {
            if (_injective != nullptr) {
                for (size_t rule_premise = 0; rule_premise != rule->premise_size(); ++rule_premise) {
                    sym::Unifier copy(unifier);
                    if (_injective->can_unify_to(rule->premise(rule_premise), copy)) {
                        ret.append(check_rule_stricter(_premises.begin(), rule, zone, parameters, copy));
                    }
                }
            } else {
                ret.append(check_rule_stricter(_premises.begin(), rule, zone, parameters, unifier));
            }
        }
        return ret;
    }
    
    void Correspondence::info(std::ostream &os) const
    {
        os << correspondence_mark << correspondence_mark_left << std::endl;
        os << correspondence_tab << correspondence_enforced_mark;
        os << correspondence_mark_separator << std::endl;
        if (_injective != nullptr) {
            os << correspondence_tab << correspondence_injective_mark << correspondence_space;
            _injective->info(os);
            os << correspondence_mark_separator << std::endl;
        }
        for (auto i = _premises.begin(); i != _premises.end(); ++i) {
            os << correspondence_tab << correspondence_premise_mark << correspondence_space;
            (*i)->info(os);
            os << correspondence_mark_separator << std::endl;
        }
        os << correspondence_tab << correspondence_conclusion_mark << correspondence_space;
        _conclusion->info(os);
        os << correspondence_mark_separator << std::endl;
        auto constraints = _zone.info();
        for (auto constraint = constraints.begin(); constraint != constraints.end(); ++constraint) {
            os << correspondence_tab << correspondence_constraint_mark << correspondence_space << *constraint << correspondence_mark_separator << std::endl;
        }
        os << correspondence_mark_right << correspondence_mark_separator << std::endl;
    }
}
