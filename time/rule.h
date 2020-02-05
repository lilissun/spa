//
//  rule.h
//  time
//
//  Created by Li Li on 18/7/14.
//  Copyright (c) 2014 Lilissun. All rights reserved.
//

#ifndef __time__rule__
#define __time__rule__

#include "common/exception.h"
#include "symbolic/model.h"
#include "symbolic/ppl.h"

namespace emc {
    class Renamer;
    class Namer;
    class Name;
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
    const std::string rule_mark = "@rule";
    const std::string rule_mark_left = "{";
    const std::string rule_mark_right = "}";
    const std::string rule_mark_separator = ";";
    const std::string rule_premise_mark = "@premise";
    const std::string rule_constraint_mark = "@constraint";
    const std::string rule_guard_mark = "@guard";
    const std::string rule_conclusion_mark = "@conclusion";
    const std::string rule_space = " ";
    const std::string rule_tab = "    ";
    
    const size_t rule_premise_sort_size = 4;

    enum RuleSelect {
        SELECTABLE,
        NONSELECTABLE,
        FINAL,
        RULE_SELECT_END,
    };
    
    class Correspondence;
    class RuleInvalid : public com::Exception {};
    class Rule : public sym::Model {
        friend Correspondence;
        
    public:
        typedef size_t Index;
        
    protected:
        std::pair<RuleSelect, Index> _select;
        std::vector<sym::Guard *> _guards; //owned
        std::vector<sym::Event *> _premises; // owned
        ppl::Zone _zone;
        sym::Event * _conclusion; // owned
        
    public:
        Rule(sym::Event *conclusion) : _guards(), _premises(), _zone(true), _conclusion(conclusion) {}
        virtual ~Rule();
        
    public:
        Rule(const Rule &) = delete;
        Rule & operator = (const Rule &) = delete;
        
    public:
        const std::pair<RuleSelect, Index> & selection() const { return _select; }
        const sym::Guard * guard(size_t index) const { return _guards.at(index); }
        size_t guard_size() const { return _guards.size(); }
        const sym::Event * premise(size_t index) const { return _premises.at(index); }
        size_t premise_size() const { return _premises.size(); }
        const sym::Event * conclusion() const { return _conclusion; }
        const ppl::Zone & zone() const { return _zone; }
        
    public:
        Rule * rename(emc::Namer &namer);
        Rule * clone() const;
        
    protected:
        void merge_premises_with_same_key();
        void zone_nontrivial() const;
        void conclusion_nontrivial() const;
        void guard_satisfiable() const;
        void remove_unrelated_when_provided(const sym::Selector &selector);
        void remove_useless_timestamps_in_zone(const std::set<emc::Name> &parameters);
        void remove_trivial_guards();
        void remove_duplicated_guards();
        void prune_premises(const sym::Pruner &pruner) const;
        void sort_premises();
        void select(const sym::Selector &selector);
        
    public:
        bool valid(const std::set<emc::Name> &parameters, const sym::Selector &selector, const sym::Pruner &pruner) {
            try {
                merge_premises_with_same_key();
                zone_nontrivial();
                conclusion_nontrivial();
                guard_satisfiable();
                remove_unrelated_when_provided(selector);
                remove_useless_timestamps_in_zone(parameters);
                remove_duplicated_guards();
                remove_trivial_guards();
                prune_premises(pruner);
                sort_premises();
                select(selector);
                return true;
            } catch (const RuleInvalid &e) {
                return false;
            }
        }
        
    public:
        Rule * add_guard(sym::Guard *guard) { _guards.push_back(guard); return this; }
        Rule * add_premise(sym::Event *premise) { _premises.push_back(premise); return this; }
        Rule * add_constraint(const sym::Constraint *constraint);
        
    protected:
        bool stricter_than_1st_step(const std::vector<sym::Event *>::const_iterator &rule_premise, const Rule *rule, const sym::Unifier &unifier) const;
        bool stricter_than_2nd_step(const std::vector<sym::Guard *>::const_iterator &rule_guard, const Rule *rule, const sym::Unifier &unifier) const;
        
    public:
        Rule * compose(const Rule *rule, emc::Namer &namer) const;
        bool imply(const Rule *rule) const;
                
    public:
        void info(std::ostream &os) const;
    };
}
#endif /* defined(__time__rule__) */
