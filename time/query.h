//
//  query.h
//  time
//
//  Created by Li Li on 5/8/15.
//  Copyright (c) 2015 Lilissun. All rights reserved.
//

#ifndef __time__query__
#define __time__query__

#include <iostream>
#include <set>
#include <vector>

#include "symbolic/ppl.h"
#include "symbolic/model.h"

namespace emc {
    class Namer;
}

namespace sym {
    class Term;
    class Guard;
    class Constraint;
    class Event;
    class Number;
    class Parameter;
    class Unifier;
    class Rewritter;
    class Builder;
    class Selector;
    class Pruner;
}

namespace tim {
    const std::string correspondence_mark = "@correspondence";
    const std::string correspondence_mark_left = "{";
    const std::string correspondence_mark_right = "}";
    const std::string correspondence_mark_separator = ";";
    const std::string correspondence_injective_mark = "@injective";
    const std::string correspondence_premise_mark = "@premise";
    const std::string correspondence_constraint_mark = "@constraint";
    const std::string correspondence_conclusion_mark = "@conclusion";
    const std::string correspondence_enforced_mark = "@enforced";
    const std::string correspondence_space = " ";
    const std::string correspondence_tab = "    ";

    enum CorrespondenceType {
        CORRESPOND_UNRELATED,
        CORRESPOND_SATISFIED,
        CORRESPOND_VIOLATED,
        CORRESPOND_TYPE_END,
    };
    
    class Rule;
    class Correspondence : public sym::Model {
    protected:
        sym::Event *_injective;
        std::vector<sym::Event *> _premises;
        ppl::Zone _zone;
        sym::Event *_conclusion;
        bool _enforced;
        
    public:
        Correspondence(std::vector<sym::Event *> premises, sym::Event *conclusion, bool enforced) : _injective(nullptr), _premises(premises), _zone(true), _conclusion(conclusion), _enforced(enforced) {}
        Correspondence(sym::Event *injective, std::vector<sym::Event *> premises, sym::Event *conclusion, bool enforced) : _injective(injective), _premises(premises), _zone(true), _conclusion(conclusion), _enforced(enforced) {}
        virtual ~Correspondence();
        
    public:
        bool enforced() const { return _enforced; }
        bool injective() const { return _injective != nullptr; }
        
    public:
        Correspondence * add_constraint(const sym::Constraint *constraint);
        
    protected:
        ppl::ZoneSet make_rule_stricter(const std::vector<sym::Event *>::const_iterator &this_premise, const Rule *rule, const std::set<emc::Name> &parameters, const sym::Unifier &unifier) const;
    public:
        ppl::ZoneSet ensure_non_injective_correspondence(const Rule *rule, const std::set<emc::Name> &parameters) const;
        
    protected:
        ppl::ZoneSet check_rule_stricter(const std::vector<sym::Event *>::const_iterator &this_premise, const Rule *rule, const ppl::Zone &zone, const std::set<emc::Name> &parameters, const sym::Unifier &unifier) const;
    public:
        ppl::ZoneSet keep_non_injective_correspondence(const Rule *rule, const ppl::Zone &zone, const std::set<emc::Name> &parameters) const;
        
    protected:
        bool check_rule_premise_stricter(const std::vector<sym::Event *>::const_iterator &this_premise, const Rule *rule, const sym::Unifier &unifier) const;
        std::vector<size_t> find_injective_correspondence(const Rule *rule) const;
    public:
        // One thing to note here:
        // In this function, we try to merge all the premises in the rule which can be unified by _injective,
        // so the injective correspondence can be checked using two rules.
        // However, this may not be true, which results in false negatives,
        // because the rule is ignored if the premises cannot be unified.
        // In fact, the correctness of our method depends on how the injective is defined.
        // In ProVerif paper, it is defined as the number of conclusions cannot exceeds the number of injectives,
        // which is more "accurate" but cannot be checked easily.
        // It needs dedicate proofs to be checked with bounded number of rules.
        // Besides, conceptually, it is not what is intended by the injective correspondence.
        // It seems that even if its proved, undesirable things can happen.
        // Maybe we need to find a better (formal) defintion for injective correspondence.
        ppl::ZoneSet ensure_injective_correspondence(const Rule *left, const Rule *right, const std::set<emc::Name> &parameters, emc::Namer &namer) const;
        
    public:
        void info(std::ostream &os) const;
    };
}

#endif /* defined(__time__query__) */
