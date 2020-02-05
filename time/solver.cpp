//
//  solver.cpp
//  time
//
//  Created by Li Li on 18/7/14.
//  Copyright (c) 2014 Lilissun. All rights reserved.
//

#include "solver.h"

#include <sstream>

#include "common/runtime.h"
#include "context.h"
#include "symbolic/constraint.h"
#include "symbolic/event.h"
#include "symbolic/unifier.h"
#include "symbolic/term.h"
#include "symbolic/ppl.h"

//#define TIME_SOLVE_DEBUG

namespace tim {
    Solver::~Solver() {
        for (auto i = _rules.begin(); i != _rules.end(); ++i) {
            for (auto j = i->_cached_rules.begin(); j != i->_cached_rules.end(); ++j) {
                delete j->first;
            }
            for (auto j = i->_staged_rules.begin(); j != i->_staged_rules.end(); ++j) {
                delete j->first;
            }
        }
        
        for (auto i = _queries.begin(); i != _queries.end(); ++i) {
            delete i->first;
        }
    }
    
    size_t Solver::cached_rule_size() const
    {
        size_t size = 0;
        for (auto i = _rules.begin(); i != _rules.end(); ++i) {
            size += i->_cached_rules.size();
        }
        return size;
    }
    
    size_t Solver::staged_rule_size() const
    {
        size_t size = 0;
        for (auto i = _rules.begin(); i != _rules.end(); ++i) {
            size += i->_staged_rules.size();
        }
        return size;
    }

    void Solver::add_parameter(const emc::Name &parameter)
    {
        auto res = _parameters.insert(parameter);
        ASSERT(res.second);
    }
    
    void Solver::add_constraint(const sym::Constraint *constraint)
    {
        ASSERT(_zones.size() == 1);
        _zones.at(0).update(constraint);
    }
    
    void Solver::add_priority_barrier()
    {
        if (_rules.back()._cached_rules.size() == 0) {
            return ;
        } else {
            _rules.push_back(Barrier());
        }
    }
    
    void Solver::add_query(Correspondence *query)
    {
        _queries.push_back(std::make_pair(query, std::vector<RulePosition>()));
    }
    
    void Solver::cache_rule(Rule *rule, RuleStatus status)
    {
        ++_rule_count;
        
#ifdef TIME_SOLVE_DEBUG
        std::cout << "Rule in cache: " << cached_rule_size() << std::endl;
        std::cout << "Rule staged: " << staged_rule_size() << std::endl;
        std::cout << "Rule before check and clean: " << std::endl;
        rule->info(std::cout);
#else
        if (_rule_count % 5 == 0) {
            std::cout << "." << std::flush;
        }
        if (_rule_count % 100 == 0) {
            std::cout << std::endl << std::flush;
        }
        if (_rule_count % 500 == 0 && _rule_count != 0) {
            std::cout << "Rule in cache: " << cached_rule_size() << std::endl << std::flush;
            std::cout << "Rule staged: " << staged_rule_size() << std::endl;
            std::cout << "Rule count: " << _rule_count << std::endl << std::flush;
        }
#endif
        if (rule->valid(_parameters, _context->selector(), _context->pruner())) {
#ifdef TIME_SOLVE_DEBUG
            std::cout << "Rule cached at priority " << status.priority() << ": " << std::endl;
            rule->info(std::cout);
            std::cout << std::endl;
#endif
            _rules.at(status.priority())._cached_rules.push_back(std::make_pair(rule, status));
        } else {
#ifdef TIME_SOLVE_DEBUG
            std::cout << "Rule invalid. " << std::endl << std::endl;
#endif
            delete rule;
        }
    }
    
    bool Solver::stage_rule(Rule * rule, RuleStatus status)
    {
#ifdef TIME_SOLVE_DEBUG
        std::cout << "Stage rule: " << std::endl;
        rule->info(std::cout);
#endif
        
        if (add_rule(rule, status)) {
            if (rule->selection().first == RuleSelect::SELECTABLE) {
                compose_selectable_rule(rule, status);
                return false;
            } else if (rule->selection().first == RuleSelect::NONSELECTABLE) {
                compose_not_selectable_rule(rule, status);
                return false;
            } else {
                ASSERT(rule->selection().first == RuleSelect::FINAL);
                _final_rules.push_back(status.position());
                return check_query(rule, status);
            }
        } else {
            delete rule;
            return false;
        }
    }
    
    bool Solver::add_rule(Rule *rule, RuleStatus &status)
    {
        ppl::ZoneSet zones = _zones;
        zones.intersect(rule->zone());
        if (zones.empty()) { // we does not add it
#ifdef TIME_SOLVE_DEBUG
            std::cout << "Rule's zone is empty considering the global zones: " << std::endl;
            std::cout << "Rule not added." << std::endl << std::endl;
#endif
            return false;
        }

        status.index(_rules.at(status.priority())._staged_rules.size());
        bool rule_deleted = false;
        for (auto i = _rules.at(status.priority())._staged_rules.begin(); i != _rules.at(status.priority())._staged_rules.end(); ++i) {
            if (i->second.valid() == false) {
                continue;
            }
            
            if (i->first->imply(rule)) {
#ifdef TIME_SOLVE_DEBUG
                std::cout << "Rule implied by: " << std::endl;
                i->first->info(std::cout);
                std::cout << "Rule not added." << std::endl << std::endl;
#endif
                ASSERT(rule_deleted == false);
                return false;
            } else if (rule->imply(i->first)) {
#ifdef TIME_SOLVE_DEBUG
                std::cout << "Rule deleted: " << std::endl;
                i->first->info(std::cout);
#endif
                // In this case, this rule must be added
                rule_deleted = true;
                i->second.replacement(status.position());
            }
        }
        
#ifdef TIME_SOLVE_DEBUG
        std::cout << "Rule added";
        if (rule->selection().first == RuleSelect::SELECTABLE) {
            std::cout << " at " << rule->selection().second;
        }
        std::cout << "." << std::endl << std::endl;
#endif
        _rules.at(status.priority())._staged_rules.push_back(std::make_pair(rule, status));
        return true;
    }
                        
    void Solver::compose_selectable_rule(Rule *rule, const RuleStatus &status)
    {
        ASSERT(rule->selection().first == RuleSelect::SELECTABLE);
        for (size_t current_priority = 0; current_priority != status.priority() + 1; ++current_priority) {
            bool priority_composed = false;
            for (auto i = _rules.at(current_priority)._staged_rules.begin(); i != _rules.at(current_priority)._staged_rules.end(); ++i) {
                if (i->second.valid() == false) {
                    continue;
                }
                
                if (i->first->selection().first == RuleSelect::NONSELECTABLE) {
                    auto comp = i->first->compose(rule, _context->namer());
                    if (comp != nullptr) {
#ifdef TIME_SOLVE_DEBUG
                        std::cout << "Compose by at " << rule->selection().second << " : " << std::endl;
                        i->first->info(std::cout);
#endif
                        priority_composed = true;
                        cache_rule(comp, RuleStatus(status.priority(), i->second.position(), status.position()));
                    }
                }
            }
            if (priority_composed) {
                break;
            }
        }
    }
    
    void Solver::compose_not_selectable_rule(Rule *rule, const RuleStatus &status)
    {
        ASSERT(rule->selection().first == RuleSelect::NONSELECTABLE);
        for (size_t current_priority = 0; current_priority != status.priority() + 1; ++current_priority) {
            bool priority_composed = false;
            for (auto i = _rules.at(current_priority)._staged_rules.begin(); i != _rules.at(current_priority)._staged_rules.end(); ++i) {
                if (i->second.valid() == false) {
                    continue;
                }
                
                if (i->first->selection().first == RuleSelect::SELECTABLE) {
                    auto comp = rule->compose(i->first, _context->namer());
                    if (comp != nullptr) {
#ifdef TIME_SOLVE_DEBUG
                        std::cout << "Compose to at " << i->first->selection().second << " : " << std::endl;
                        i->first->info(std::cout);
#endif
                        priority_composed = true;
                        cache_rule(comp, RuleStatus(status.priority(), status.position(), i->second.position()));
                    }
                }
            }
            if (priority_composed) {
                break;
            }
        }
    }

    bool Solver::check_query(Rule *rule, const RuleStatus &status)
    {
        ASSERT(rule->selection().first == RuleSelect::FINAL);
#ifdef TIME_SOLVE_DEBUG
        std::cout << "Check rule against queries: " << std::endl;
        rule->info(std::cout);
        std::cout << "with parameter relation:" << std::endl;
        auto zone = rule->zone();
        zone.retain(_parameters);
        std::cout << zone << std::endl;
        std::cout << "Before checking:" << std::endl;
        output_zones();
#endif
        for (auto q = _queries.begin(); q != _queries.end(); ++q) {
            auto zones = q->first->ensure_non_injective_correspondence(rule, _parameters);
#ifdef TIME_SOLVE_DEBUG
            std::cout << "Multiply zones:" << std::endl;
            std::cout << zones << std::endl;
#endif
            _zones.multiply(zones);
#ifdef TIME_SOLVE_DEBUG
            std::cout << "Ater multiplication:" << std::endl;
            output_zones();
#endif
        }
#ifdef TIME_SOLVE_DEBUG
        std::cout << "After checking:" << std::endl;
        output_zones();
        std::cout << std::endl;
#endif

        return _zones.empty();
    }
    
    std::pair<SolverResult, std::string> Solver::solve()
    {
        if (_queries.empty()) {
            return std::make_pair(SolverResult::NO_QUERY, "");
        }

        // ensure non-injective correspondence
        for (size_t priority = 0; priority != _rules.size(); ++priority) {
            while (_rules.at(priority)._cached_rules.empty() == false) {
                auto rule = _rules.at(priority)._cached_rules.front();
                _rules.at(priority)._cached_rules.pop_front();
                if (stage_rule(rule.first, rule.second)) {
                    return std::make_pair(SolverResult::ATTACK_FOUND, "fail to ensure non-injective correspondence");
                }
            }
        }
        
        // ensure injective correspondence
        for (auto query = _queries.begin(); query != _queries.end(); ++query) {
            if (query->first->injective()) {
                for (auto i = _final_rules.begin(); i != _final_rules.end(); ++i) {
                    for (auto j = i; j != _final_rules.end(); ++j) {
                        auto zones = query->first->ensure_injective_correspondence(_rules.at(i->first)._staged_rules.at(i->second).first, _rules.at(j->first)._staged_rules.at(j->second).first, _parameters, _context->namer());
                        _zones.multiply(zones);
                        if (_zones.empty()) {
                            return std::make_pair(SolverResult::ATTACK_FOUND, "fail to ensure injective correspondence");
                        }
                    }
                }
            }
        }
        
        // every correspondence query has a satisfying rule in final rules if enforced
        ppl::ZoneSet zones(true);
        for (auto query = _queries.begin(); query != _queries.end(); ++query) {
            if (query->first->enforced()) {
                ppl::ZoneSet zones_of_query(false);
                for (size_t zone_index = 0; zone_index != _zones.size(); ++zone_index) {
                    for (auto position = _final_rules.begin(); position != _final_rules.end(); ++position) {
                        auto rule = _rules.at(position->first)._staged_rules.at(position->second);
                        ASSERT(rule.first->selection().first == RuleSelect::FINAL);
                        if (rule.second.valid() == false) {
                            continue;
                        }
                        auto zones_of_query_of_rule = query->first->keep_non_injective_correspondence(rule.first, _zones.at(zone_index), _parameters);
#ifdef TIME_SOLVE_DEBUG
                        if (zones_of_query_of_rule.empty() == false) {
                            std::cout << "Check rule: " << std::endl;
                            rule.first->info(std::cout);
                            std::cout << "against query: " << std::endl;
                            query->first->info(std::cout);
                            std::cout << "Zone before update:" << std::endl;
                            std::cout << _zones.at(zone_index);
                            std::cout << "Updated zones:" << std::endl;
                            std::cout << zones_of_query_of_rule;
                        }
#endif
                        if (zones_of_query_of_rule.empty() == false) {
                            query->second.push_back(*position);
                        }
                        zones_of_query.append(zones_of_query_of_rule);
                    }
                }
                zones.multiply(zones_of_query);
                if (zones.empty()) {
                    _zones = zones;
                    ASSERT(query->second.empty());
                    return std::make_pair(SolverResult::ATTACK_FOUND, "fail to ensure one rule for every enforced correspondence");
                }
            }
        }
        
        _zones = zones;
        return std::make_pair(SolverResult::SECURE, "");
    }
    
    void Solver::output_rules(std::ostream &os) const
    {
        os << "General Rules: " << std::endl;
        size_t count = 0;
        for (size_t priority = 0; priority != _rules.size(); ++priority) {
            for (auto i = _rules.at(priority)._staged_rules.begin(); i != _rules.at(priority)._staged_rules.end(); ++i) {
                if (i->first->selection().first == RuleSelect::SELECTABLE) {
                    os << i->second;
                    os << "Selectable at " << i->first->selection().second << ":" << std::endl;
                    i->first->info(os);
                    std::cout << std::endl;
                    ++count;
                } else if (i->first->selection().first == RuleSelect::NONSELECTABLE) {
                    os << i->second;
                    os << "Not Selectable : " << std::endl;
                    i->first->info(os);
                    std::cout << std::endl;
                    ++count;
                }
            }
        }
        if (count == 0) {
            os << "[@empty]" << std::endl;
            std::cout << std::endl;
        }
    }
    
    void Solver::output_finals(std::ostream &os) const
    {
        os << "Final Rules: " << std::endl;
        size_t count = 0;
        for (size_t priority = 0; priority != _rules.size(); ++priority) {
            for (auto i = _rules.at(priority)._staged_rules.begin(); i != _rules.at(priority)._staged_rules.end(); ++i) {
                if (i->first->selection().first == RuleSelect::FINAL) {
                    os << i->second;
                    i->first->info(os);
                    os << "with parameter relation:" << std::endl;
                    auto zone = i->first->zone();
                    zone.retain(_parameters);
                    os << zone;
                    os << std::endl;
                    ++count;
                }
            }
        }
        if (count == 0) {
            os << "[@empty]" << std::endl;
            std::cout << std::endl;
        }
    }
    
    size_t Solver::size() const
    {
        size_t size = 0;
        for (size_t priority = 0; priority != _rules.size(); ++priority) {
            size += _rules.at(priority)._cached_rules.size();
            size += _rules.at(priority)._staged_rules.size();
        }
        return size;
    }
    
    void Solver::output_zones() const
    {
        if (_zones.empty()) {
            std::cout << "Cannot find any parameter relation satisfying the requested properties." << std::endl;
            std::cout << std::endl;
        } else {
            for (size_t i = 0; i != _zones.size(); ++i) {
                std::cout << i << ". ";
                std::cout << "Requested parameter relation:" << std::endl;
                std::cout << _zones.at(i);
                std::cout << std::endl;
            }
        }
    }
}