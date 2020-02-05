//
//  solver.h
//  time
//
//  Created by Li Li on 18/7/14.
//  Copyright (c) 2014 Lilissun. All rights reserved.
//

#ifndef __time__solver__
#define __time__solver__

#include <iostream>

#include <vector>
#include <list>
#include <map>
#include <set>

#include "explicit/name.h"
#include "explicit/some.h"
#include "symbolic/ppl.h"
#include "content.h"
#include "rule.h"
#include "query.h"

namespace sym {
    class Event;
    class Constraint;
}

namespace tim {
    class Context;
    
    enum SolverResult {
        UNKNOWN,
        NO_QUERY, 
        ATTACK_FOUND,
        SECURE,
        SOLVER_RESULT_END,
    };
    
    typedef size_t RuleIndex;
    typedef size_t RulePriority;
    typedef std::pair<RulePriority, RuleIndex> RulePosition;
    class RuleStatus {
    protected:
        RulePriority _priority;
        emc::Some<RuleIndex> _index;
        emc::Some<RulePosition> _replacement;
        emc::Some<std::pair<RulePosition, RulePosition>> _origin;
        
    public:
        RuleStatus(RulePriority priority)
        : _priority(priority), _index(), _replacement(), _origin() {}
        RuleStatus(RulePriority priority, RulePosition first, RulePosition second)
        : _priority(priority), _index(), _replacement(), _origin(std::make_pair(first, second)) {}
        RuleStatus(const RuleStatus &status)
        : _priority(status._priority), _index(status._index)
        , _replacement(status._replacement), _origin(status._origin) {}
        
    public:
        RulePriority priority() const { return _priority; }
        void index(RuleIndex index) { _index.value(index); }
        RulePosition position() const { return std::make_pair(_priority, _index.value()); }
        bool valid() const { return _replacement.empty(); }
        void replacement(RulePosition position) { _replacement.value(position); }
        bool initial() const { return _origin.empty(); }
        std::pair<RulePosition, RulePosition> origin() const { return _origin.value(); }
        void origin(RulePosition first, RulePosition second) { _origin.value(std::make_pair(first, second)); }
        
        friend std::ostream & operator << (std::ostream &os, const RuleStatus &status)
        {
            os << "Rule No. [" << status._priority << "." << status._index.value() << "]" << std::endl;
            if (status._replacement.empty()) {
                os << "Valid Rule" << std::endl;
            } else {
                os << "Replaced by Rule [";
                os << status._replacement.value().first << ".";
                os << status._replacement.value().second << "]" << std::endl;
            }
            if (status._origin.empty()) {
                os << "Initial Rule" << std::endl;
            } else {
                os << "Composed by Rule [";
                os << status._origin.value().first.first << ".";
                os << status._origin.value().first.second << "] and [";
                os << status._origin.value().second.first << ".";
                os << status._origin.value().second.second << "]" << std::endl;
            }
            return os;
        }
    };
    
    class Solver : public Content {
    protected:
        struct Barrier {
            std::list<std::pair<Rule *, RuleStatus>> _cached_rules; // owned
            std::vector<std::pair<Rule *, RuleStatus>> _staged_rules; // owned
        };
        std::set<emc::Name> _parameters;
        ppl::ZoneSet _zones;
        std::vector<Barrier> _rules;
        std::vector<RulePosition> _final_rules;
        std::vector<std::pair<Correspondence *, std::vector<RulePosition>>> _queries;
        size_t _rule_count;
        std::pair<SolverResult, std::string> _result;
        
    public:
        Solver(Context *context)
        : Content(context), _zones(true), _rules({Barrier()}), _final_rules(), _queries(), _rule_count(0), _result(std::make_pair(SolverResult::UNKNOWN, "")) { }
        ~Solver();
        
    public:
        Solver(const Solver &) = delete;
        Solver & operator = (const Solver &) = delete;
        
    protected:
        size_t cached_rule_size() const;
        size_t staged_rule_size() const;
        
    public:
        void add_parameter(const emc::Name &parameter);
        void add_constraint(const sym::Constraint *constraint);
        void add_priority_barrier();
        void cache_rule(Rule *rule) { cache_rule(rule, RuleStatus(_rules.size() - 1)); }
        void add_query(Correspondence *query);
        
    public:
        std::pair<SolverResult, std::string> solve();
        
    protected:
        void cache_rule(Rule *rule, RuleStatus status);
        bool stage_rule(Rule *rule, RuleStatus status);
        bool add_rule(Rule *rule, RuleStatus &status);
        void compose_selectable_rule(Rule *rule, const RuleStatus &status);
        void compose_not_selectable_rule(Rule *rule, const RuleStatus &status);
        bool check_query(Rule *query, const RuleStatus &status);
        
    public:
        void output_rules(std::ostream &os) const;
        void output_finals(std::ostream &os) const;
        void output_zones() const;
        
    public:
        size_t size() const;
        size_t rule_count() const { return _rule_count; }
    };
}

#endif /* defined(__time__solver__) */
