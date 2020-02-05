//
//  builder.cpp
//  time
//
//  Created by Li Li on 9/7/14.
//  Copyright (c) 2014 Lilissun. All rights reserved.
//

#include "builder.h"

#include <sstream>

#include "common/runtime.h"
#include "common/location.h"
#include "explicit/namer.h"
#include "symbolic/nonce.h"
#include "symbolic/variable.h"
#include "symbolic/function.h"
#include "symbolic/tuple.h"
#include "symbolic/list.h"
#include "symbolic/name.h"
#include "symbolic/number.h"
#include "symbolic/constant.h"
#include "symbolic/value.h"
#include "symbolic/event.h"
#include "symbolic/guard.h"
#include "symbolic/constraint.h"
#include "symbolic/state.h"
#include "context.h"

namespace tim {
    sym::Nonce * Builder::nonce(const std::string &name, const com::Location &location)
    {
        auto it = _name_cache.find(name);
        if (it == _name_cache.end()) {
            sym::Nonce * term = sym::make_nonce(name, _context->namer(), _indexer);
            auto res = _name_cache.insert(std::make_pair(name, std::make_pair(sym::TermType::TERM_NONCE, location)));
            ASSERT(res.second);
            return term;
        } else {
            if (it->second.first == sym::TermType::TERM_NONCE) {
                return sym::make_nonce(name, _context->namer(), _indexer);
            } else {
                std::stringstream ss;
                ss << "nonce " << name << " regonized at " << location;
                ss << " has conflict type at " << it->second.second;
                USER_ERROR(ss.str());
            }
        }
    }

    sym::Variable * Builder::variable(const std::string &name, const com::Location &location)
    {
        auto it = _name_cache.find(name);
        if (it == _name_cache.end()) {
            sym::Variable * term = sym::make_variable(name, _context->namer(), _indexer);
            auto res = _name_cache.insert(std::make_pair(name, std::make_pair(term->type(), location)));
            ASSERT(res.second);
            return term;
        } else {
            if (it->second.first == sym::TermType::TERM_VARIABLE) {
                return sym::make_variable(name, _context->namer(), _indexer);
            } else {
                std::stringstream ss;
                ss << "variable " << name << " regonized at " << location;
                ss << " has conflict type at " << it->second.second;
                USER_ERROR(ss.str());
            }
        }
    }
    
    sym::Number * Builder::timestamp(const std::string &name, const com::Location &location)
    {
        auto it = _name_cache.find(name);
        if (it == _name_cache.end()) {
            sym::Number * term = sym::make_number(name, _context->namer(), _indexer);
            auto res = _name_cache.insert(std::make_pair(name, std::make_pair(term->type(), location)));
            ASSERT(res.second);
            return term;
        } else {
            if (it->second.first == sym::TermType::TERM_NUMBER) {
                return sym::make_number(name, _context->namer(), _indexer);
            } else {
                std::stringstream ss;
                ss << "timestamp " << name << " regonized at " << location;
                ss << " has conflict type at " << it->second.second;
                USER_ERROR(ss.str());
            }
        }
    }
    
    sym::Nonce * Builder::nonce(const std::string &name, emc::Name::ID id, const com::Location &location)
    {
        if (_context->namer().passed(id)) {
            return sym::make_nonce(name, id, _context->namer());
        } else {
            std::stringstream ss;
            ss << "nonce " << name << " regonized at " << location;
            ss << " has an id that may be duplicated with other newly generated names";
            USER_ERROR(ss.str());
        }
    }
    
    sym::Variable * Builder::variable(const std::string &name, emc::Name::ID id, const com::Location &location)
    {
        if (_context->namer().passed(id)) {
            return sym::make_variable(name, id, _context->namer());
        } else {
            std::stringstream ss;
            ss << "variable " << name << " regonized at " << location;
            ss << " has an id that may be duplicated with other newly generated names";
            USER_ERROR(ss.str());
        }
    }
    
    sym::Number * Builder::timestamp(const std::string &name, emc::Name::ID id, const com::Location &location)
    {
        if (_context->namer().passed(id)) {
            return sym::make_number(name, id, _context->namer());;
        } else {
            std::stringstream ss;
            ss << "timestamp " << name << " regonized at " << location;
            ss << " has an id that may be duplicated with other newly generated names";
            USER_ERROR(ss.str());
        }
    }
    
    sym::Value * Builder::value(int value)
    {
        return sym::make_value(value);
    }
    
    sym::Tuple * Builder::tuple(const std::vector<sym::Term *> arguments)
    {
        return sym::make_tuple(arguments);
    }
    
    sym::List * Builder::list(const std::vector<sym::Term *> arguments)
    {
        return sym::make_list(arguments, sym::make_variable("x", _context->namer()));
    }
    
    sym::Name * Builder::string(const std::string &name, const com::Location &location)
    {
        auto it = _string_decls.find(name);
        if (it == _string_decls.end()) {
            std::stringstream ss;
            ss << "string " << name << " used at " << location;
            ss << " has never been declared before";
            USER_ERROR(ss.str());
        } else {
            return sym::make_name(name, _context->namer());
        }
    }
    
    sym::Constant * Builder::parameter(const std::string &name, const com::Location &location)
    {
        auto it = _parameter_decls.find(name);
        if (it == _parameter_decls.end()) {
            std::stringstream ss;
            ss << "parameter " << name << " regonized at " << location;
            ss << " has has not been declared before";
            USER_ERROR(ss.str());
        } else {
            return sym::make_constant(name, _context->namer());
        }
    }
    
    void Builder::declare_parameter(const std::string &name, const com::Location &location)
    {
        auto res = _parameter_decls.insert(std::make_pair(name, location));
        if (res.second == false) {
            std::stringstream ss;
            ss << "parameter " << name << " declared at " << location;
            ss << " has been declared before at " << res.first->second;
            USER_ERROR(ss.str());
        } else {
            _context->solver().add_parameter(_context->namer().constant(name));
        }
    }
    
    sym::Function * Builder::function(const std::string &name, sym::Tuple *arguments, const com::Location &location)
    {
        auto it = _function_decls.find(name);
        if (it == _function_decls.end()) {
            std::stringstream ss;
            ss << "function " << name << " used at " << location;
            ss << " has never been declared before";
            USER_ERROR(ss.str());
        } else if (it->second.first != arguments->size()) {
            std::stringstream ss;
            ss << "function " << name << " constructed at " << location;
            ss << " has been declared with different argument number at " << it->second.second;
            USER_ERROR(ss.str());
        } else {
            return sym::make_function(name, arguments, _context->namer());
        }
    }
    
    void Builder::declare_function(const std::string &name, size_t size, const com::Location &location)
    {
        auto res = _function_decls.insert(std::make_pair(name, std::make_pair(size, location)));
        if (res.second == false) {
            std::stringstream ss;
            ss << "function " << name << " declared at " << location;
            ss << " has already been declared before at " << res.first->second.second;
            USER_ERROR(ss.str());
        }
    }
    
    void Builder::declare_string(const std::string &name, const com::Location &location)
    {
        auto res = _string_decls.insert(std::make_pair(name, location));
        if (res.second == false) {
            std::stringstream ss;
            ss << "string " << name << " declared at " << location;
            ss << " has already been declared before at " << res.first->second;
            USER_ERROR(ss.str());
        }
    }
    
    sym::Function * Builder::plus(sym::Term *left, sym::Term *right)
    {
        return sym::make_plus(left, right);
    }
    
    sym::Function * Builder::minus(sym::Term *left, sym::Term *right)
    {
        return sym::make_minus(left, right);
    }
    
    sym::Function * Builder::multiply(sym::Term *left, sym::Term *right)
    {
        return sym::make_multiply(left, right);
    }
    
    sym::Constraint * Builder::less(sym::Term *left, sym::Term *right)
    {
        return sym::make_constraint_less(left, right);
    }

    sym::Constraint * Builder::less_equal(sym::Term *left, sym::Term *right)
    {
        return sym::make_constraint_less_equal(left, right);
    }

    sym::Constraint * Builder::greater(sym::Term *left, sym::Term *right)
    {
        return sym::make_constraint_greater(left, right);
    }

    sym::Constraint * Builder::greater_equal(sym::Term *left, sym::Term *right)
    {
        return sym::make_constraint_greater_equal(left, right);
    }

    sym::Constraint * Builder::equal(sym::Term *left, sym::Term *right)
    {
        return sym::make_constraint_equal(left, right);
    }
    
    sym::Event * Builder::event(const std::string &name, sym::Tuple *arguments, const com::Location &location)
    {
        auto it = _event_decls.find(name);
        if (it == _event_decls.end()) {
            std::stringstream ss;
            ss << "event " << name << " used at " << location;
            ss << " has not been declared before";
            USER_ERROR(ss.str());
        } else if (it->second.first.second != arguments->size()) {
            std::stringstream ss;
            ss << "event " << name << " constructed at " << location;
            ss << " has been declared with different argument number at " << it->second.second;
            USER_ERROR(ss.str());
        } else {
            return sym::make_event(name, it->second.first.first, arguments, _context->namer());
        }
    }
    
    emc::Name Builder::event_name(const std::string &name, const com::Location &location)
    {
        auto it = _event_decls.find(name);
        if (it == _event_decls.end()) {
            std::stringstream ss;
            ss << "event " << name << " used at " << location;
            ss << " has not been declared before";
            USER_ERROR(ss.str());
        } else {
            return _context->namer().constant(name);
        }
    }
    
    void Builder::declare_event(const std::string &name, size_t keycount, size_t size, const com::Location &location)
    {
        auto res = _event_decls.insert(std::make_pair(name, std::make_pair(std::make_pair(keycount, size), location)));
        if (res.second == false) {
            std::stringstream ss;
            ss << "event " << name << " declared at " << location;
            ss << " has already been declared before at " << res.first->second.second;
            USER_ERROR(ss.str());
        }
    }
    
    sym::Guard * Builder::guard_not_equal(sym::Term *left, sym::Term *right)
    {
        return sym::make_guard_not_equal(left, right);
    }
    
    sym::Guard * Builder::guard_not_unifiable_to(sym::Term *left, sym::Term *right)
    {
        return sym::make_guard_not_unifiable_to(left, right);
    }
}