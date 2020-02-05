//
//  ppl.cpp
//  symbolic
//
//  Created by Li Li on 3/5/16.
//  Copyright © 2016 Lilissun. All rights reserved.
//

#include "ppl.h"

#include "common/vector.h"
#include "common/runtime.h"

#include "explicit/renamer.h"

#include "symbolic/term.h"
#include "symbolic/function.h"
#include "symbolic/tuple.h"
#include "symbolic/constant.h"
#include "symbolic/number.h"
#include "symbolic/value.h"
#include "symbolic/constraint.h"
#include "symbolic/unifier.h"

namespace Parma_Polyhedra_Library {
    std::vector<Constraint> inverse(const Constraint &constraint)
    {
        if (constraint.is_tautological()) {
            return std::vector<Constraint>({Constraint::zero_dim_false()});
        } else if (constraint.is_inconsistent()) {
            return std::vector<Constraint>({Constraint::zero_dim_positivity()});
        }
        
        // linear_expression (constraint_type [ == or >= or > ]) 0
        Linear_Expression expr;
        for (dimension_type i = 0; i != constraint.space_dimension(); ++i) {
            expr += constraint.coefficient(Variable(i)) * Variable(i);
        }
        expr += constraint.inhomogeneous_term();
        
        if (constraint.is_equality()) {
            return std::vector<Constraint>({expr > 0, expr < 0});
        } else if (constraint.is_nonstrict_inequality()) {
            return std::vector<Constraint>({expr < 0});
        } else {
            ASSERT(constraint.is_strict_inequality());
            return std::vector<Constraint>({expr <= 0});
        }
    }
    
    std::string linear_constraint_info(const Constraint &constraint, const std::vector<emc::Name> &names)
    {
        std::stringstream left_stream;
        bool left_empty = true;
        std::stringstream right_stream;
        bool right_empty = true;
        
        for (PPL::dimension_type i = 0; i != constraint.space_dimension(); ++i) {
            PPL::Coefficient cf = constraint.coefficient(PPL::Variable(i));
            if (cf == 0) {
                continue;
            }
            
            bool update_left = true;
            if (cf < 0) {
                cf = -1 * cf;
                update_left = false;
            }
            
            std::stringstream &ss = update_left ? left_stream : right_stream;
            bool ss_empty = update_left ? left_empty : right_empty;
            
            if (ss_empty == false) {
                ss << " + ";
            }
            
            if (cf != 1) {
                ss << cf << " * ";
            }
            
            if (names.at(i).type() == emc::NameType::VARIABLE) {
                ss << ppl::zone_variable_mark << names.at(i);
            } else {
                ASSERT(names.at(i).type() == emc::NameType::CONSTANT);
                ss << ppl::zone_constant_mark << names.at(i);
            }
            
            if (update_left) {
                left_empty = false;
            } else {
                right_empty = false;
            }
        }
        
        PPL::Coefficient cf = constraint.inhomogeneous_term();
        if (cf != 0) {
            bool update_left = true;
            if (cf < 0) {
                cf = -1 * cf;
                update_left = false;
            }
            
            std::stringstream &ss = update_left ? left_stream : right_stream;
            bool ss_empty = update_left ? left_empty : right_empty;
            
            if (ss_empty == false) {
                ss << " + ";
            }
            ss << cf;
            
            if (update_left) {
                left_empty = false;
            } else {
                right_empty = false;
            }
        }
        
        if (left_empty) {
            left_stream << "0";
        }
        
        if (right_empty) {
            right_stream << "0";
        }
        
        if (constraint.is_equality()) {
            left_stream << " == " << right_stream.rdbuf();
        } else if (constraint.is_nonstrict_inequality()) {
            left_stream << " >= " << right_stream.rdbuf();
        } else {
            ASSERT(constraint.is_strict_inequality());
            left_stream << " > " << right_stream.rdbuf();
        }
        
        return left_stream.str();
    }
}

namespace ppl {
    PPL::Coefficient coefficient(const sym::Term *term)
    {
        ASSERT(term->type() == sym::TermType::TERM_VALUE);
        return ((const sym::Value *)term)->value().get_int();
    }
    
    sym::Value * coefficient(const PPL::Coefficient &coefficient)
    {
        return sym::make_value(coefficient.get_si());
    }
    
    PPL::Variable variable(const sym::Term *term, const std::map<emc::Name, Zone::Dimension> &name_map)
    {
        if (term->type() == sym::TermType::TERM_NUMBER) {
            ASSERT(term->name().type() == emc::NameType::VARIABLE);
        } else {
            ASSERT(term->type() == sym::TermType::TERM_CONSTANT);
            ASSERT(term->name().type() == emc::NameType::CONSTANT);
        }
        auto number = (sym::Number *)term;
        return PPL::Variable(name_map.at(number->name()));
    }
    
    sym::Term * variable(PPL::dimension_type index, const std::vector<emc::Name> &names)
    {
        if (names.at(index).type() == emc::NameType::VARIABLE) {
            return sym::make_number(names.at(index));
        } else {
            ASSERT(names.at(index).type() == emc::NameType::CONSTANT);
            return sym::make_constant(names.at(index));
        }
    }
    
    PPL::Linear_Expression linear_expression(const sym::Term *term, const std::map<emc::Name, Zone::Dimension> &name_map)
    {
        switch (term->type()) {
            case sym::TermType::TERM_NUMBER:
            case sym::TermType::TERM_CONSTANT:
                return variable(term, name_map);
                
            case sym::TermType::TERM_FUNCTION:
            {
                auto function = (const sym::Function *)term;
                switch (function->op()) {
                    case sym::FunctionType::FUNCTION_PLUS:
                        ASSERT(function->arguments()->size() == 2);
                        return linear_expression(function->arguments()->at(0), name_map) + linear_expression(function->arguments()->at(1), name_map);
                        
                    case sym::FunctionType::FUNCTION_MINUS:
                        ASSERT(function->arguments()->size() == 2);
                        return linear_expression(function->arguments()->at(0), name_map) - linear_expression(function->arguments()->at(1), name_map);
                        
                    case sym::FunctionType::FUNCTION_MULTIPLY:
                        ASSERT(function->arguments()->size() == 2);
                        if ((function->arguments()->at(0)->type() == sym::TermType::TERM_NUMBER || function->arguments()->at(0)->type() == sym::TermType::TERM_CONSTANT) && function->arguments()->at(1)->type() == sym::TermType::TERM_VALUE) {
                            return variable(function->arguments()->at(0), name_map) * coefficient(function->arguments()->at(1));
                        } else if ((function->arguments()->at(1)->type() == sym::TermType::TERM_NUMBER || function->arguments()->at(1)->type() == sym::TermType::TERM_CONSTANT) && function->arguments()->at(0)->type() == sym::TermType::TERM_VALUE) {
                            return variable(function->arguments()->at(1), name_map) * coefficient(function->arguments()->at(0));
                        } else {
                            ASSERT(false);
                        }
                    default:
                        ASSERT(false);
                }
            }
                
            case sym::TermType::TERM_VALUE:
                return PPL::Linear_Expression(coefficient(term));
                
            default:
                ASSERT(false);
        }
    }
    
    sym::Constraint * linear_constraint(const PPL::Constraint &constraint, const std::vector<emc::Name> &names)
    {
        sym::Term *left = nullptr;
        sym::Term *right = nullptr;
        for (PPL::dimension_type i = 0; i != constraint.space_dimension(); ++i) {
            PPL::Coefficient cf = constraint.coefficient(PPL::Variable(i));
            if (cf == 0) {
                continue;
            }
            
            sym::Term * expression = nullptr;
            bool update_left = true;
            if (cf > 0) {
                expression = left;
            } else {
                expression = right;
                cf = -1 * cf;
                update_left = false;
            }
            
            sym::Term *node = nullptr;
            if (cf != 1) {
                node = sym::make_multiply(coefficient(cf), variable(i, names));
            } else {
                node = variable(i, names);
            }
            
            if (expression == nullptr) {
                expression = node;
            } else {
                expression = sym::make_plus(expression, node);
            }
            
            if (update_left) {
                left = expression;
            } else {
                right = expression;
            }
        }
        
        PPL::Coefficient cf = constraint.inhomogeneous_term();
        if (cf != 0) {
            sym::Term * expression = nullptr;
            bool update_left = true;
            if (cf > 0) {
                expression = left;
            } else {
                expression = right;
                cf = -1 * cf;
                update_left = false;
            }
            
            if (expression == nullptr) {
                expression = coefficient(cf);
            } else {
                expression = sym::make_plus(expression, coefficient(cf));
            }
            
            if (update_left) {
                left = expression;
            } else {
                right = expression;
            }
        }
        
        if (left == nullptr) {
            left = sym::make_value(0);
        }
        
        if (right == nullptr) {
            right = sym::make_value(0);
        }
        
        if (constraint.is_equality()) {
            return sym::make_constraint_equal(left, right);
        } else if (constraint.is_nonstrict_inequality()) {
            return sym::make_constraint_greater_equal(left, right);
        } else {
            ASSERT(constraint.is_strict_inequality());
            return sym::make_constraint_greater(left, right);
        }
    }

    PPL::Constraint linear_constraint(const sym::Constraint *constraint, const std::vector<emc::Name> &names)
    {
        std::map<emc::Name, PPL::dimension_type> name_map;
        for (auto name = names.begin(); name != names.end(); ++name) {
            auto res = name_map.insert(std::make_pair(*name, std::distance(names.begin(), name)));
            ASSERT(res.second);
        }
        
        auto left = linear_expression(constraint->left(), name_map);
        auto right = linear_expression(constraint->right(), name_map);
        switch (constraint->op()) {
            case sym::ConstraintType::CONSTRAINT_EQUAL:
                return left == right;
                
            case sym::ConstraintType::CONSTRAINT_LESS:
                return left < right;
                
            case sym::ConstraintType::CONSTRAINT_LESS_EQUAL:
                return left <= right;
                
            case sym::ConstraintType::CONSTRAINT_GREATER:
                return left > right;
                
            case sym::ConstraintType::CONSTRAINT_GREATER_EQUAL:
                return left >= right;
                
            default:
                ASSERT(false);
        }
    }
    
    std::vector<PPL::Constraint> linear_constraints(const sym::Constraint *constraint, const std::vector<emc::Name> &names)
    {
        std::map<emc::Name, PPL::dimension_type> name_map;
        for (auto name = names.begin(); name != names.end(); ++name) {
            auto res = name_map.insert(std::make_pair(*name, std::distance(names.begin(), name)));
            ASSERT(res.second);
        }
        
        auto left = linear_expression(constraint->left(), name_map);
        auto right = linear_expression(constraint->right(), name_map);
        switch (constraint->op()) {
            case sym::ConstraintType::CONSTRAINT_EQUAL:
                return {left == right};
                
            case sym::ConstraintType::CONSTRAINT_NOT_EQUAL:
                return {left > right, left < right};
                
            case sym::ConstraintType::CONSTRAINT_LESS:
                return {left < right};
                
            case sym::ConstraintType::CONSTRAINT_LESS_EQUAL:
                return {left <= right};
                
            case sym::ConstraintType::CONSTRAINT_GREATER:
                return {left > right};
                
            case sym::ConstraintType::CONSTRAINT_GREATER_EQUAL:
                return {left >= right};
                
            default:
                ASSERT(false);
        }
    }
    
    void Zone::add(const emc::Name &name) {
        if (std::find(_names.begin(), _names.end(), name) != _names.end()) { return ; } // ignore if name exists
        _zone.add_space_dimensions_and_embed(1);
        _names.push_back(name);
    }
    
    void Zone::remove(const emc::Name &name) {
        auto it = std::find(_names.begin(), _names.end(), name);
        if (it == _names.end()) { return ; } // ignore if name does not exist
        Dimension index = std::distance(_names.begin(), it);
        Dimension size = _zone.space_dimension();
        if (index != size - 1) {
            PPL::Partial_Function function;
            for (Dimension i = 0; i != size; ++i) {
                if (i == index) {
                    function.insert(i, size - 1);
                } else if (i == size - 1) {
                    function.insert(i, index);
                } else {
                    function.insert(i, i);
                }
            }
            _zone.map_space_dimensions(function);
            _names.at(index) = _names.back();
        }
        _zone.remove_higher_space_dimensions(size - 1);
        _names.pop_back();
    }
    
    void Zone::retain(const std::set<emc::Name> &names) {
        std::vector<emc::Name> original_names(_names);
        for (auto i = original_names.begin(); i != original_names.end(); ++i) {
            if (names.find(*i) == names.end()) {
                remove(*i);
            }
        }
    }
    
    void Zone::rename(const emc::Name &from, const emc::Name &to) {
        if (from == to) { return ; } // ignore if from and to are the same
        auto it_from = std::find(_names.begin(), _names.end(), from);
        if (it_from == _names.end()) { return ; } // ignore if from does not exist
        auto it_to = std::find(_names.begin(), _names.end(), to);
        if (it_to == _names.end()) {
            *it_from = to;
        } else {
            Dimension dim_from = std::distance(_names.begin(), it_from);
            Dimension dim_to = std::distance(_names.begin(), it_to);
            _zone.add_constraint(PPL::Variable(dim_from) == PPL::Variable(dim_to));
            remove(from);
        }
    }
    
    void Zone::rename(const sym::Unifier &unifier)
    {
        auto name_map = unifier.simple_map(_names);
        for (auto i = name_map.begin(); i != name_map.end(); ++i) {
            rename(i->first, i->second);
        }
    }
    
    void Zone::rename(const emc::Renamer &renamer)
    {
        for (auto i = renamer.map().begin(); i != renamer.map().end(); ++i) {
            rename(i->first, i->second);
        }
    }
    
    void Zone::reorder_names(const std::vector<emc::Name> &names) {
        // assuming names are unique, belonging to _names
        ASSERT(names.size() == _names.size());
        PPL::Partial_Function function;
        for (auto from = _names.begin(); from != _names.end(); ++from) {
            auto to = std::find(names.begin(), names.end(), *from);
            ASSERT(to != names.end());
            function.insert(std::distance(_names.begin(), from), std::distance(names.begin(), to));
        }
        _zone.map_space_dimensions(function);
        _names = names;
    }
    
    void Zone::update(const sym::Constraint *constraint)
    {
        auto names = constraint->get_subnames(sym::TermType::TERM_CONSTANT|sym::TermType::TERM_NUMBER);
        add(names.begin(), names.end());
        _zone.add_constraint(linear_constraint(constraint, _names));
    }
    
    bool Zone::contain(const Zone &zone) const {
        Zone this_copy(*this);
        this_copy.add(zone._names.begin(), zone._names.end());
        Zone zone_copy(zone);
        zone_copy.add(_names.begin(), _names.end());
        this_copy.reorder_names(zone_copy._names);
        return this_copy._zone.contains(zone_copy._zone);
    }
    
    ZoneSet Zone::make_contain(const Zone &zone, const std::set<emc::Name> names) const {
        Zone this_copy(*this);
        this_copy.add(zone._names.begin(), zone._names.end());
        Zone zone_copy(zone);
        zone_copy.add(_names.begin(), _names.end());
        zone_copy.reorder_names(this_copy._names);
        
        ZoneSet ret_zones(true);
        auto constraints = _zone.minimized_constraints();
        for (auto constraint = constraints.begin(); constraint != constraints.end(); ++constraint) {
            auto inversed_constraints = inverse(*constraint);
            for (auto inversed_constraint = inversed_constraints.begin(); inversed_constraint != inversed_constraints.end(); ++inversed_constraint) {
                auto empty_zone(zone_copy);
                empty_zone._zone.add_constraint(*inversed_constraint);
                ZoneSet valid_zones = empty_zone.make_empty(names);
                ret_zones.multiply(valid_zones);
            }
        }
        return ret_zones;
    }
    
    ZoneSet Zone::make_empty(const std::set<emc::Name> names) const {
        ZoneSet ret_zones(false);
        Zone copy = *this;
        copy.retain(names);
        PPL::Constraint_System cs = copy._zone.minimized_constraints();
        for (auto constraint = cs.begin(); constraint != cs.end(); ++constraint) {
            std::vector<PPL::Constraint> inversed_constraints = inverse(*constraint);
            for (auto inversed_constraint = inversed_constraints.begin(); inversed_constraint != inversed_constraints.end(); ++inversed_constraint) {
                Zone inversed_zone(copy._names);
                inversed_zone._zone.add_constraint(*inversed_constraint);
                ret_zones.insert(inversed_zone);
            }
        }
        return ret_zones;
    }
    
    void Zone::intersect(const Zone &zone) {
        if(zone.empty()) {
            _zone = PPL::Zone(0, PPL::EMPTY);
            return ;
        } else if (zone.universe()) {
            return ;
        } else if (empty()) {
            return ;
        } else if (universe()) {
            _names = zone._names;
            _zone = zone._zone;
        }
        
        Zone zone_copy(zone);
        zone_copy.add(_names.begin(), _names.end());
        add(zone._names.begin(), zone._names.end());
        reorder_names(zone_copy._names);
        _zone.intersection_assign(zone_copy._zone);
    }
    
    bool Zone::equal(const Zone &zone) const
    {
        if (_names.size() != zone._names.size()) {
            return false;
        }
        
        for (auto name = _names.begin(); name != _names.end(); ++name) {
            if (std::find(zone._names.begin(), zone._names.end(), *name) == zone._names.end()) {
                return false;
            }
        }
        
        Zone copy(*this);
        copy.reorder_names(zone._names);
        return copy._zone == zone._zone;
    }
    
    std::ostream & operator << (std::ostream &os, const Zone &zone)
    {
        if (zone.empty()) {
            os << zone_empty << std::endl;
        } else if (zone.universe()) {
            os << zone_universe << std::endl;
        } else {
            auto constraints = zone.info();
            for (auto constraint = constraints.begin(); constraint != constraints.end(); ++constraint) {
                os << *constraint << std::endl;
            }
        }
        return os;
    }
    
    std::vector<std::string> Zone::info() const
    {
        std::vector<std::string> constraints;
        PPL::Constraint_System cs = _zone.minimized_constraints();
        for (auto constraint = cs.begin(); constraint != cs.end(); ++constraint) {
            constraints.push_back(PPL::linear_constraint_info(*constraint, _names));
        }
        return constraints;
    }
    
    void Zone::print_info() const
    {
        std::cout << *this << std::flush;
    }
        
    void ZoneSet::insert(const Zone &zone)
    {
        if (zone.empty()) {
            return ;
        } else if (zone.universe()) {
            _zones.clear();
            _zones.push_back(zone);
        }
        
        auto i = _zones.begin();
        bool removed = false;
        while (i != _zones.end()) {
            if (i->contain(zone)) {
                ASSERT(removed == false);
                return ;
            } else if (zone.contain(*i)) {
                removed = true;
                i = _zones.erase(i);
            } else {
                ++i;
            }
        }
        _zones.push_back(zone);
    }
    
    void ZoneSet::intersect(const Zone &zone)
    {
        ZoneSet target(false);
        for (auto i = _zones.begin(); i != _zones.end(); ++i) {
            i->intersect(zone);
            target.insert(*i);
        }
        _zones.swap(target._zones);
    }
    
    void ZoneSet::multiply(const ZoneSet &zones)
    {
        if (zones.empty()) {
            _zones.clear();
            return ;
        } else if (zones.universe()) {
            return ;
        }
        
        ZoneSet target(false);
        for (auto i = _zones.begin(); i != _zones.end(); ++i) {
            for (auto j = zones._zones.begin(); j != zones._zones.end(); ++j) {
                Zone newzone(*i);
                newzone.intersect(*j);
                target.insert(newzone);
            }
        }
        _zones.swap(target._zones);
    }
    
    void ZoneSet::append(const ZoneSet &zones)
    {
        for (auto i = zones._zones.begin(); i != zones._zones.end(); ++i) {
            insert(*i);
        }
    }
    
    std::ostream & operator << (std::ostream &os, const ZoneSet &zones)
    {
        if (zones._zones.empty()) {
            std::cout << zoneset_empty << std::endl;
        } else {
            for (size_t i = 0; i != zones._zones.size(); ++i) {
                std::cout << "No. " << (i + 1) << " Zone:" << std::endl;
                std::cout << zones._zones.at(i);
                std::cout << std::endl;
            }
        }
        return os;
    }
    
    void ZoneSet::print_info() const
    {
        std::cout << *this;
    }
}