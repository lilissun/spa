//
//  builder.h
//  time
//
//  Created by Li Li on 9/7/14.
//  Copyright (c) 2014 Lilissun. All rights reserved.
//

#ifndef __time__builder__
#define __time__builder__

#include <iostream>
#include <map>
#include <vector>
#include <set>

#include "common/location.h"
#include "explicit/name.h"
#include "explicit/indexer.h"
#include "symbolic/term.h"
#include "content.h"
#include "helper.h"

namespace emc {
    class Namer;
}

namespace sym {
    class Nonce;
    class Variable;
    class Number;
    class Constant;
    class Function;
    class Tuple;
    class List;
    class Name;
    class Value;
    class Binary;
    class Event;
    class Guard;
    class Constraint;
}

namespace tim {
    class Context;
    class Builder : public Content {
    protected: // global declarations
        std::map<std::string, std::pair<size_t, com::Location>> _function_decls;
        std::map<std::string, com::Location> _string_decls;
        std::map<std::string, std::pair<std::pair<size_t, size_t>, com::Location>> _event_decls;
        std::map<std::string, com::Location> _parameter_decls;
        
    protected: // local name usages
        std::map<std::string, std::pair<sym::TermType, com::Location>> _name_cache;
        emc::Indexer _indexer;
        
    public:
        Builder(Context *context) : Content(context) {}
        virtual ~Builder() {}
        
    public:
        Builder(const Builder &builder) = delete;
        Builder & operator = (const Builder &builder) = delete;

    public:
        void clear() {
            _name_cache.clear();
            _indexer.clear();
        }
        
    public:
        sym::Nonce * nonce(const std::string &name, const com::Location &location);
        sym::Variable * variable(const std::string &name, const com::Location &location);
        sym::Number * timestamp(const std::string &name, const com::Location &location);

    public:
        sym::Nonce * nonce(const std::string &name, emc::Name::ID id, const com::Location &location);
        sym::Variable * variable(const std::string &name, emc::Name::ID id, const com::Location &location);
        sym::Number * timestamp(const std::string &name, emc::Name::ID id, const com::Location &location);
        
    public:
        void declare_parameter(const std::string &name, const com::Location &location);
        void declare_string(const std::string &name, const com::Location &location);
        void declare_function(const std::string &name, size_t size, const com::Location &location);
        
    public:
        sym::Constant * parameter(const std::string &name, const com::Location &location);
        sym::Name * string(const std::string &name, const com::Location &location);
        sym::Function * function(const std::string &name, sym::Tuple *arguments, const com::Location &location);
        sym::Tuple * tuple(const std::vector<sym::Term *> arguments);
        sym::List * list(const std::vector<sym::Term *> arguments);
        sym::Value * value(int value);
        
    public:
        sym::Function *plus(sym::Term *left, sym::Term *right);
        sym::Function *minus(sym::Term *left, sym::Term *right);
        sym::Function *multiply(sym::Term *left, sym::Term *right);
        
    public:
        sym::Constraint *less(sym::Term *left, sym::Term *right);
        sym::Constraint *less_equal(sym::Term *left, sym::Term *right);
        sym::Constraint *greater(sym::Term *left, sym::Term *right);
        sym::Constraint *greater_equal(sym::Term *left, sym::Term *right);
        sym::Constraint *equal(sym::Term *left, sym::Term *right);
        
    public:
        void declare_event(const std::string &name, size_t keycount, size_t size, const com::Location &location);
        emc::Name event_name(const std::string &name, const com::Location &location);
        sym::Event * event(const std::string &name, sym::Tuple *arguments, const com::Location &location);
        
    public:
        sym::Guard * guard_not_equal(sym::Term *left, sym::Term *right);
        sym::Guard * guard_not_unifiable_to(sym::Term *left, sym::Term *right);
    };
}

#endif /* defined(__time__builder__) */
