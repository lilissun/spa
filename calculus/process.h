//
//  process.h
//  calculus
//
//  Created by Li Li on 27/5/15.
//  Copyright (c) 2015 Lilissun. All rights reserved.
//

#ifndef __calculus__process__
#define __calculus__process__

#include <iostream>
#include <vector>
#include <iomanip>

#include "common/location.h"
#include "symbolic/model.h"

namespace sym {
    class Term;
}

namespace tim {
    class Rule;
}

namespace cal {
    enum ProcessType {
        PROCESS_NULL,
        PROCESS_PARALLEL,
        PROCESS_REPLICATION,
        PROCESS_NONCE,
        PROCESS_TIMESTAMP,
        PROCESS_UNIQUE,
        PROCESS_IF,
        PROCESS_WAIT,
        PROCESS_LET,
        PROCESS_INPUT,
        PROCESS_OUTPUT,
        PROCESS_INIT,
        PROCESS_JOIN,
        PROCESS_ACCEPT,
        PROCESS_SECRECY,
        PROCESS_PROCESS,
        PROCESS_CALL,
        PROCESS_TYPE_END,
    };
    
    const std::string process_null = "@null";
    const std::string process_parallel = "@parallel";
    const std::string process_bar = "|";
    const std::string process_replication = "!";
    const std::string process_nonce = "@new @nonce";
    const std::string process_at = "@at";
    const std::string process_unique = "@unique";
    const std::string process_in = "@in";
    const std::string process_timestamp = "@new @timestamp";
    const std::string process_if = "@if";
    const std::string process_then = "@then";
    const std::string process_else = "@else";
    const std::string process_wait = "@wait";
    const std::string process_until = "@until";
    const std::string process_let = "@let";
    const std::string process_input = "@receive";
    const std::string process_output = "@send";
    const std::string process_init = "@init";
    const std::string process_join = "@join";
    const std::string process_accept = "@accept";
    const std::string process_secrecy = "@secrecy";
    const std::string process_open = "@open";
    const std::string process_process = "@process";
    const std::string process_invoke = "@invoke";
    const std::string process_engage = "@engage";
    const std::string process_system = "@system";
    const std::string process_function = "@function";
    const std::string process_require = "@require";
    const std::string process_private = "@private";
    
    const std::string process_left_para = "(";
    const std::string process_right_para = ")";
    const std::string process_left_bracket = "{";
    const std::string process_right_bracket = "}";
    const std::string process_equal = "=";
    const std::string process_endline = ";";
    const std::string process_comma = ",";
    const std::string process_space = " ";
    const size_t process_tab_size = 4;
    
    class Program;
    class State;
    class Expression;
    class Function;
    class Name;
    class Value;
    class Identifier;
    class Process : public sym::Model {
    protected:
        ProcessType _type;
        com::Location _location;
        
    public:
        Process(ProcessType type, const com::Location &location) : _type(type), _location(location) {}
        virtual ~Process() {}
        
    public:
        ProcessType type() const { return _type; }
        
    public:
        virtual void execute(Program *program, const State *state, std::vector<tim::Rule *> &rules) const = 0;
        
    public:
        virtual void info(const size_t depth, std::ostream &os) const = 0;
        virtual void info(std::ostream &os) const;
    };
}

#endif /* defined(__calculus__process__) */
